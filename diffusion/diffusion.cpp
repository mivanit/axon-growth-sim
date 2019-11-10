// diffusion.cpp

#include "diffusion.h"

/*******************************************************************/

template<typename T>
Diffusion<T>::Diffusion(const unsigned int dim) : dim_{ dim } {
    u_ = init_2d(dim);
}

/*******************************************************************/

template<typename T>
Diffusion<T>::Diffusion(const unsigned int dim, const T D, const T dx,
                        const T dy) : dim_{ dim }, D_{ D }, dx_{ dx },
                        dy_{ dy } {
    u_ = init_2d(dim);
}

/*******************************************************************/

template<typename T>
Diffusion<T>::Diffusion(const unsigned int dim, const T D, const T dx,
                        const T dy, const T** u) : dim_{ dim }, D_{ D },
                        dx_{ dx }, dy_{ dy } {
    u_ = new T*[dim_];
    for (unsigned int i = 0; i < dim_; ++i) {
        u_[i] = new T[dim_];
        for (unsigned int j = 0; j < dim_; ++j) {
            u_[i][j] = u[i][j];
        }
    }
}

/*******************************************************************/

template<typename T>
Diffusion<T>::~Diffusion() {
    clean_2d(u_, dim_);
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::set(const unsigned int row, const unsigned int col,
                       const T val) const {
    u_[row][col] = val;
}

/*******************************************************************/

template<typename T>
T Diffusion<T>::concentration(const unsigned int row,
                              const unsigned int col) const {
    return u_[row][col];
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::ftcs(const T t_start, const T dt, const T t_end) {
    // Scaling constants
    const T ax = D_ * dt / (dx_ * dx_);
    const T ay = D_ * dt / (dy_ * dy_);
    
    unsigned int t_steps = (unsigned int) floor((t_end - t_start) / dt);
    for (unsigned int t = 0; t < t_steps; ++t) {
        ftcs_step(ax, ay, dt);
    }
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::ftcs_step(const T ax, const T ay, const T dt) {
    // Initialize the temporary grid
    T** ut = init_2d(dim_);
    
    // Update concentrations at each grid point
    for (unsigned int i = 0; i < dim_; ++i) {
        for (unsigned int j = 0; j < dim_; ++j) {
            // boundary conditions
            if (i == 0 || i == dim_ - 1 || j == 0 || j == dim_ - 1) {
                ut[i][j] = 0;
            } else { // interior point
                ut[i][j] = (1 - decay_ * dt) * u_[i][j] +
                           ax * (u_[i][j - 1] - 2 * u_[i][j] + u_[i][j + 1]) +
                           ay * (u_[i - 1][j] - 2 * u_[i][j] + u_[i + 1][j]);
            }
        }
    }
    
    for (unsigned int i = 0; i < dim_; ++i) {
        for (unsigned int j = 0; j < dim_; ++j) {
            u_[i][j] = ut[i][j];
        }
    }
    
    // Clean up
    clean_2d(ut, dim_);
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::adi(const T t_start, const T dt, const T t_end) {
    // Scaling constants
    const T ax = D_ * dt / (dx_ * dx_);
    const T ay = D_ * dt / (dy_ * dy_);
    
    // Note that since we are using Dirichlet BCs, we only need to
    // solve linear systems at the interior points. So the coefficient
    // matrices only should be of dimension (dim_ - 2) x (dim_ - 2).
    
    // Coefficient matrices
    T** AX = init_2d(dim_ - 2);  // first half of time step
    T** AY = init_2d(dim_ - 2);  // second half of time step
    for (unsigned int i = 0; i < dim_ - 2; ++i) {
        if (i != 0) {
            AX[i][i - 1] = -ax /2;
            AY[i][i - 1] = -ay / 2;
        }
        AX[i][i] = 1 + ax;
        AY[i][i] = 1 + ay;
        if (i != dim_ - 1) {
            AX[i][i + 1] = -ax / 2;
            AY[i][i + 1] = -ay / 2;
        }
    }
    const T** AXc = const_cast<const T**>(AX);
    const T** AYc = const_cast<const T**>(AY);

    unsigned int t_steps = (unsigned int) floor((t_end - t_start) / dt);
    for (unsigned int t = 0; t < t_steps; ++t) {
        adi_step(AXc, AYc, ax, ay, dt);
    }
    
    // Clean up
    clean_2d(AX, dim_);
    clean_2d(AY, dim_);
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::adi_step(const T**& AX, const T**& AY, const T ax,
                            const T ay, const T dt) {
    // Initialize the temporary grid
    T** ut = init_2d(dim_);
    
    // NOTE: Due to the Dirichlet BCs, we only need to solve linear
    // systems for the interior points.
    
    // x sweep
    for (unsigned int j = 1; j < dim_ - 1; ++j) {
        // Create the righthand matrix
        T* B = new T[dim_ - 2];
        // Shift x-coordinate for linear system
        for (unsigned int i = 1; i < dim_ - 1; ++i) {
            B[i - 1] = ay/2 * (u_[i][j - 1] + u_[i][j + 1]) +
                       (1 - ay - decay_ * dt / 2.0) * u_[i][j];
        }
        const T* Bc = const_cast<const T*>(B);
        
        // Solve the system and assign to intermediate grid
        T* soln = new T[dim_ - 2];
        tri_solve(AX, Bc, soln, dim_ - 2);
        for (unsigned int i = 0; i < dim_ - 2; ++i) {
            // Shift back x-coordinate
            ut[i + 1][j] = soln[i];
        }
        
        // Clean up
        delete[] soln;
        delete[] B;
    }

    // y sweep
    for (unsigned int i = 1; i < dim_ - 1; ++i) {
        // Create the righthand matrix
        T* B = new T[dim_ - 2];
        // Shift y-coordinate for linear system
        for (unsigned int j = 1; j < dim_ - 1; ++j) {
            B[j - 1] = ax/2 * (ut[i - 1][j] + ut[i + 1][j]) +
                       (1 - ax - decay_ * dt / 2.0) * ut[i][j];
        }
        const T* Bc = const_cast<const T*>(B);
        
        // Solve the system and assign to original grid
        T* soln = new T[dim_ - 2];
        tri_solve(AY, Bc, soln, dim_ - 2);
        for (unsigned int j = 0; j < dim_ - 2; ++j) {
            // Shift back y-coordinate
            u_[i][j + 1] = soln[j];
        }
        
        // Clean up
        delete[] soln;
        delete[] B;
    }
    
    // Clean up
    clean_2d(ut, dim_);
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::tri_solve(const T**& A, const T*& B, T*& soln,
                             const unsigned int dim) const {
    T cp[dim - 2];
    cp[0] = A[0][1] / A[0][0];
    for (unsigned int i = 1; i < dim - 1; ++i) {
        cp[i] = A[i][i + 1]/(A[i][i] - A[i][i - 1] * cp[i - 1]);
    }
    
    T dp[dim - 1];
    dp[0] = B[0] / A[0][0];
    for (unsigned int i = 1; i < dim; ++i) {
        dp[i] = (B[i] - A[i][i - 1] * dp[i - 1]) / (A[i][i] - A[i][i - 1] * cp[i - 1]);
    }
    
    soln[dim - 1] = dp[dim - 1];
    // Shift indices to prevent unsigned int underflow
    for (unsigned int i = dim - 1; i > 0; --i) {
        soln[i - 1] = dp[i - 1] - cp[i - 1] * soln[i];
    }
}

/*******************************************************************/

template<typename T>
bool Diffusion<T>::add_source(const unsigned int row,
                              const unsigned int col,
                              const T strength) {
    Coord c(row, col);
    auto itr = sources_.find(c);
    if (itr != sources_.end()) {
        itr->second = strength;
        return false;
    } else {
        sources_.insert({c, strength});
        return true;
    }
}

/*******************************************************************/

template<typename T>
bool Diffusion<T>::remove_source(const unsigned int row,
                                 const unsigned int col) {
    auto itr = sources_.find(Coord(row, col));
    if (itr == sources_.end()) {
        return false;
    } else {
        sources_.erase(itr);
        return true;
    }
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::fire(unsigned int row, unsigned int col) {
    u_[row][col] += sources_[Coord(row, col)];
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::write_to_csv(const std::string& file) const {
    std::ofstream ofs(file);
    if (!ofs.is_open()) {
        std::cerr << "Error opening " << file << ", aborting."
                  << std::endl;
    } else {
        ofs.precision(_PRECISION_);
        for (unsigned int i = 0; i < dim_; ++i) {
            for (unsigned int j = 0; j < dim_; ++j) {
                ofs << std::to_string(u_[j][i]) << ",";
            }
            ofs << std::endl;
        }
    }
}

/*******************************************************************/

template<typename T>
T** Diffusion<T>::init_2d(unsigned int dim) const {
    T** M = new T*[dim_];
    for (unsigned int i = 0; i < dim_; ++i) {
        M[i] = new T[dim_];
        for (unsigned int j = 0; j < dim_; ++j) {
            M[i][j] = 0;
        }
    }
    return M;
}

/*******************************************************************/

template<typename T>
void Diffusion<T>::clean_2d(T**& X, unsigned int dim) const {
    for (unsigned int i = 0; i < dim; ++i) {
        delete[] X[i];
    }
    delete[] X;
}

/*******************************************************************/

// Explicit template instantiations
template class Diffusion<double>;
template class Diffusion<float>;
template class Diffusion<long>;

/*******************************************************************/
