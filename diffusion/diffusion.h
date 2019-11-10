/**************************************************************************
 *** Models 2D diffusion with homogeneous Dirichlet boundary conditions ***
 **************************************************************************/

#ifndef diffusion_h
#define diffusion_h

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <cmath>

// Decimal precision for output to CSV
#define _PRECISION_ 8

// Typedef for coordinate pair of unsigned ints
typedef std::pair<unsigned int, unsigned int> Coord;

// Class is explicitly instantiated in diffusion.cpp. Allowed types are
// double, float, & long. These define the precision used in all calculations.

template<typename T>
class Diffusion {
private:
	// Grid to store concentrations at each point
    T** u_;
    // The sources/sinks in the grid
    std::map<Coord, T> sources_;
    
    /*** Parameters ***/
    // Dimension of the grid (dim x dim)
    const unsigned int dim_;
    // Grid squares are of size dx x dy
    const T dx_ = 1, dy_ = 1;
    // Diffusion coefficient
    const T D_ = 1;
    // Decay rate (between 0 and 1, per timestep^(-1))
    T decay_ = 0;
    
    // Performs a single forward time central space (FTCS) step. First two
    // arguments are the scaling constants which depend on dt.
    void ftcs_step(const T ax, const T ay, const T dt);
    
    // Performs a single alternating direction implicit scheme (ADI) step. First
    // two arguments are the coefficient matrices for each half of the time step.
    // Second two arguments are the scaling constants which depend on dt.
    void adi_step(const T**& AX, const T**& AY, const T ax, const T ay, const T dt);
    
    // Implements the tridiagonal matrix algorithm: solves exactly the matrix
    // equation Ax=B where dim(A) = dim x dim, dim(B) = dim(soln) = dim x 1,
    // and A is tridiagonal. Computes solution in O(n) time, assigns to soln.
    void tri_solve(const T**& A, const T*& B, T*& soln, const unsigned int dim) const;
    
    // Creates a matrix of 0's with dimensions dim x dim
    T** init_2d(unsigned int dim) const;

    // Deletes a dynamically allocated 2D array with dim rows. A deep delete
    // is not performed on the rows.
    void clean_2d(T**& X, unsigned int dim) const;

public:
    // Initializes grid to all zeroes with dimensions of dim x dim with D = 1
    Diffusion(const unsigned int dim);
    
    // Initializes grid to all zeroes with dimensions of (dim dx) x (dim dy)
    Diffusion(const unsigned int dim, const T D, const T dx, const T dy);
    
    // Instantiates grid with u. u must have dimensions dim x dim. The rest
    // of the parameters are the same as in the default constructor.
    Diffusion(const unsigned int dim, const T D, const T dx, const T dy,
              const T** u);
    
    // Destructor
    ~Diffusion();

	// Return the dimension of the grid
    unsigned int dim() const { return dim_; }

	// Set the concentration at a point (r,c)
    void set(const unsigned int row, const unsigned int col,
             const T val) const;
    
	// Get the concentration at a point (r,c)
    T concentration(const unsigned int row, const unsigned int col) const;
    
    // Performs FTCS algorithm from t_start to t_end with timestep dt
    void ftcs(const T t_start, const T dt, const T t_end);
    
    // Performs ADI algorithm from t_start to t_end with timestep dt
    void adi(const T t_start, const T dt, const T t_end);
    
    // Write the grid in CSV form to file
    void write_to_csv(const std::string& file) const;
    
    // Add a source to the grid. If source already exists, update the
    // strength and return false. Return true if source was added.
    bool add_source(const unsigned int row, const unsigned int col,
                    const T strength);
    
    // Remove a source from the grid. Returns true if a source was removed,
    // false if no source existed at the provided coordinates.
    bool remove_source(const unsigned int row, const unsigned int col);
    
    // Fire a source
    void fire(const unsigned int row, const unsigned int col);
    
    // Set strength of decay
    void set_decay(const T decay) { decay_ = decay; }
};
#endif
