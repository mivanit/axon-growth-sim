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

class Diffusion {
private:
	// Grid to store concentrations at each point
    double** u_;
    // The sources/sinks in the grid
    std::map<Coord, double> sources_;
    
    // Dimension of the grid (dim x dim)
    const unsigned int dim_;
    // Grid squares are of size dx x dy
    const double dx_, dy_, dt_;
    // Diffusion coefficient
    const double D_;
    // Decay rate (between 0 and 1, per timestep^(-1))
    double decay_ = 0;
    // Constants and coefficient matrices
    const double ax_, ay_;
    double** AX_;
    double** AY_;
    
    // Implements the tridiagonal matrix algorithm: solves exactly the matrix
    // equation Ax=B where dim(A) = dim x dim, dim(B) = dim(soln) = dim x 1,
    // and A is tridiagonal. Computes solution in O(n) time, assigns to soln.
    void tri_solve(double**& A, double*& B, double*& soln,
                   const unsigned int dim) const;
    
    // Creates a matrix of 0's with dimensions dim x dim
    double** init_2d(unsigned int dim);

    // Deletes a dynamically allocated 2D array with dim rows. A deep delete
    // is not performed on the rows.
    void clean_2d(double**& X, unsigned int dim);

public:
    // Initializes grid to all zeroes with dimensions of (dim dx) x (dim dy)
    Diffusion(const unsigned int dim, const double D, const double dx,
              const double dy, const double dt);
    
    // Destructor
    ~Diffusion();
    
    // Performs a single step of the ADI scheme
    void adi_step();

	// Return the dimension of the grid
    unsigned int dim() const { return dim_; }

	// Set the concentration at a point
    void set(const unsigned int row, const unsigned int col, const double val);
    
	// Get the concentration at a point
    double concentration(const unsigned int row, const unsigned int col) const;
    
    // Write the grid in CSV form to file
    void write_to_csv(const std::string& file) const;
    
    // Add a source to the grid. If source already exists, update the
    // strength and return false. Return true if source was added.
    bool add_source(const unsigned int row, const unsigned int col,
                    const double strength);
    
    // Remove a source from the grid. Returns true if a source was removed,
    // false if no source existed at the provided coordinates.
    bool remove_source(const unsigned int row, const unsigned int col);
    
    // Fire a source
    void fire(const unsigned int row, const unsigned int col);
    
    // Set strength of decay rate
    void set_decay(const double decay) { decay_ = decay; }
};
#endif
