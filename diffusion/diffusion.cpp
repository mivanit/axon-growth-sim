// diffusion.cpp

#include "diffusion.h"

/*******************************************************************/

Diffusion::Diffusion(const unsigned int dim, const double D, const double dx,
                     const double dy, const double dt, const std::string l) :
                     dim_{ dim }, D_{ D }, dx_{ dx }, dy_{ dy }, dt_{ dt },
                     ax_{ D * dt / (dx * dx) }, ay_{ D * dt / (dy * dy) },
                     LABEL_ { l } {
    u_ = init_2d(dim);
                         
    // Note that since we are using Dirichlet BCs, we only need to
    // solve linear systems at the interior points. So the coefficient
    // matrices only should be of dimension (dim_ - 2) x (dim_ - 2).
                            
    // Coefficient matrices
    AX_ = init_2d(dim_ - 2);  // first half of time step
    AY_ = init_2d(dim_ - 2);  // second half of time step
    for (unsigned int i = 0; i < dim_ - 2; ++i) {
        if (i != 0) {
            AX_[i][i - 1] = -ax_ / 2;
            AY_[i][i - 1] = -ay_ / 2;
        }
        AX_[i][i] = 1 + ax_;
        AY_[i][i] = 1 + ay_;
        if (i != dim_ - 1) {
            AX_[i][i + 1] = -ax_ / 2;
            AY_[i][i + 1] = -ay_ / 2;
        }
    }
}

/*******************************************************************/

Diffusion::~Diffusion() {
    clean_2d(AX_, dim_);
    clean_2d(AY_, dim_);
    clean_2d(u_, dim_);
}

/*******************************************************************/

void Diffusion::set(const unsigned int row, const unsigned int col,
                    const double val) {
    u_[row][col] = val;
}

void Diffusion::Crd_set(const Coord in_crd, const double val) {
    if ( Crd_valid(in_crd) )
    {
        u_[in_crd.ix()][in_crd.iy()] = val;
    }
}

void Diffusion::Crd_add(const Coord in_crd, const double val) {
    if ( Crd_valid(in_crd) )
    {
        u_[in_crd.ix()][in_crd.iy()] += val;
    }
}

/*******************************************************************/

const bool Diffusion::Crd_valid(const Coord in_crd) {
    return (
        in_crd.ix() > 1
        && in_crd.iy() > 1
        && in_crd.ix() + 1 < dim_
        && in_crd.iy() + 1 < dim_
    );
}

const double Diffusion::concentration(const unsigned int row, const unsigned int col) const {
    return u_[row][col];
}

const double Diffusion::Crd_getC(const Coord in_crd) {
    // check validity
    if (
        in_crd.ix() < 1
        || in_crd.ix() + 1 >= dim_
        || in_crd.iy() < 1
        || in_crd.iy() + 1 >= dim_
    )
    {
        return 0.0;
    }

    
    // return value
    return u_[in_crd.ix()][in_crd.iy()];
}

// use bilinear interpolation to get more precise value of concentration at a floating point coord
const double Diffusion::getC_bilin(const Coord crd) {
    // check validity
    if (
        !Crd_valid(crd)
        ||
        !Crd_valid(Coord( crd.bx()+1, crd.by()+1 ))
    ) {
        return 0.0;
    }
    
    /*
        +y +x -->
        |
        V

        a   u |     b
            * |
        ------+------
              |
        c   v |     d
    */

    // values at 4 points a,b,c,d:
    const float a = u_[crd.bx()  ][crd.by()  ];
    const float b = u_[crd.bx()+1][crd.by()  ];
    const float c = u_[crd.bx()  ][crd.by()+1];
    const float d = u_[crd.bx()+1][crd.by()+1];

    // find exact positions x,y
    const float x = fmod(crd.x,1);
    const float y = fmod(crd.y,1);

    // linearly interpolate between a,b and c,d by x to get u,v
    const float u = lin_interp(a,b,x);
    const float v = lin_interp(c,d,x);

    // linearly interpolate between u,v by y to get result `*`
    const float out = lin_interp(u,v,y);

    return out;
}

/*******************************************************************/

void Diffusion::adi_step() {
    // Initialize the temporary grid
    double** ut = init_2d(dim_);
    
    // Note that since we are using Dirichlet BCs, we only need to
    // solve linear systems for the interior points.
    
    // x sweep
    for (unsigned int j = 1; j < dim_ - 1; ++j) {
        // Create the righthand matrix
        double* B = new double[dim_ - 2];
        // Shift x-coordinate for linear system
        for (unsigned int i = 1; i < dim_ - 1; ++i) {
            B[i - 1] = ay_ / 2 * (u_[i][j - 1] + u_[i][j + 1]) +
                       (1 - ay_ - decay_ * dt_ / 2.0) * u_[i][j];
        }
        
        // Solve the system and assign to intermediate grid
        double* soln = new double[dim_ - 2];
        tri_solve(AX_, B, soln, dim_ - 2);
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
        double* B = new double[dim_ - 2];
        // Shift y-coordinate for linear system
        for (unsigned int j = 1; j < dim_ - 1; ++j) {
            B[j - 1] = ax_ / 2 * (ut[i - 1][j] + ut[i + 1][j]) +
                       (1 - ax_ - decay_ * dt_ / 2.0) * ut[i][j];
        }
        
        // Solve the system and assign to original grid
        double* soln = new double[dim_ - 2];
        tri_solve(AY_, B, soln, dim_ - 2);
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

void Diffusion::tri_solve(double**& A, double*& B, double*& soln,
                          const unsigned int dim) const {
    double cp[dim - 2];
    cp[0] = A[0][1] / A[0][0];
    for (unsigned int i = 1; i < dim - 1; ++i) {
        cp[i] = A[i][i + 1]/(A[i][i] - A[i][i - 1] * cp[i - 1]);
    }
    
    double dp[dim - 1];
    dp[0] = B[0] / A[0][0];
    for (unsigned int i = 1; i < dim; ++i) {
        dp[i] = (B[i] - A[i][i - 1] * dp[i - 1]) /
                (A[i][i] - A[i][i - 1] * cp[i - 1]);
    }
    
    soln[dim - 1] = dp[dim - 1];
    // Shift indices to prevent unsigned int underflow
    for (unsigned int i = dim - 1; i > 0; --i) {
        soln[i - 1] = dp[i - 1] - cp[i - 1] * soln[i];
    }
}

/*******************************************************************/

bool Diffusion::add_source(const unsigned int row, const unsigned int col,
                           const double strength) {
    iCoord c(row, col);
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

bool Diffusion::remove_source(const unsigned int row,
                              const unsigned int col) {
    auto itr = sources_.find(iCoord(row, col));
    if (itr == sources_.end()) {
        return false;
    } else {
        sources_.erase(itr);
        return true;
    }
}

/*******************************************************************/

void Diffusion::fire(unsigned int row, unsigned int col) {
    u_[row][col] += sources_[iCoord(row, col)];
}

/*******************************************************************/

void Diffusion::write_to_csv(const std::string& file) const {
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

double** Diffusion::init_2d(unsigned int dim) {
    double** M = new double*[dim_];
    for (unsigned int i = 0; i < dim_; ++i) {
        M[i] = new double[dim_];
        for (unsigned int j = 0; j < dim_; ++j) {
            M[i][j] = 0;
        }
    }
    return M;
}

/*******************************************************************/

void Diffusion::clean_2d(double**& X, unsigned int dim) {
    for (unsigned int i = 0; i < dim; ++i) {
        delete[] X[i];
    }
    delete[] X;
}

/*******************************************************************/
