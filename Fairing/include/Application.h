/*
 * Implicit Fairing of Arbitrary Meshes using Diffusion and Curvature Flow
 * Mathieu Desbrun, Mark Meyer, Peter Schröder, Alan H. Barr
 * ACM Siggraph '99 Proceedings
 */

#ifndef DDG_APPLICATION_H
#define DDG_APPLICATION_H

#include "../terrain-generation.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif



#include "Mesh.h"
#include "Real.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "DiscreteExteriorCalculus.h"

namespace DDG
{
   class Application
   {
   public:
      void run(const double step, Mesh& mesh)
      {
         // diagonal matrix for our mesh that transforms 0-forms for each 
         // vertex x to the associated discrete 2-forms in the dual mesh.
         SparseMatrix<Real> star0;
         HodgeStar0Form<Real>::build( mesh, star0 );

         // builds a diagonal matrix mapping primal discrete 1-forms
         // to dual discrete 1-forms
         SparseMatrix<Real> star1;
         HodgeStar1Form<Real>::build( mesh, star1 );
         
         SparseMatrix<Real> d0;
         ExteriorDerivative0Form<Real>::build( mesh, d0 );
         
         SparseMatrix<Real> L = d0.transpose() * star1 * d0; 
         SparseMatrix<Real> A = star0 + Real(step) * L;
         
         DenseMatrix<Real> x;
         getPositions(mesh, x);
         DenseMatrix<Real> rhs = star0 * x; 

         solvePositiveDefinite(A, x, rhs);
         setPositions(x, mesh);

         

            
      }

      void testing(const double step, Mesh& mesh)
      {
         
         DenseMatrix<Real> x;
         getPositions(mesh, x);
         
         SparseMatrix<Real> d0, star0, star1, Delta, Laplace, Identity;
         
         ExteriorDerivative0Form<Real>::build( mesh, d0 );
         HodgeStar0Form<Real>::build( mesh, star0 );
         HodgeStar1Form<Real>::build( mesh, star1 );
         Identity = Identity.identity(mesh.vertices.size());
         
         // Laplace is *d*d. The d*d* term is zero.
         // Then add Identity
         Laplace = star0.inverse() * (Real(step) * d0.transpose() * star1 * d0); 
         Delta = Identity + Laplace; 
         
         solveSymmetric(Delta, x, x);
         setPositions(x, mesh);
            
      }
      
   protected:
      void getPositions(const Mesh& mesh, DenseMatrix<Real>& x) const
      {
         x = DenseMatrix<Real>( mesh.vertices.size(), 3 );
         for ( VertexCIter v = mesh.vertices.begin();
              v != mesh.vertices.end();
              v ++)
         {
            for( int i = 0; i < 3; ++i)
               x(v->index, i) = v->position[i];
         }
      }
      
      void setPositions(const DenseMatrix<Real>& x, Mesh& mesh)
      {
         for ( VertexIter v = mesh.vertices.begin();
              v != mesh.vertices.end();
              v ++)
         {
            v->position = Vector(x(v->index, 0),
                                 x(v->index, 1),
                                 x(v->index, 2));
         }
      }

// Code here is included for Final Project Credit Only:
// First steps of building of a Mesh object out of Perlin Noise
// In the current state, compiler errors impede authors testing efforts.

      // void CreateMesh(){
      //    int width = 100, height = 100;
      //    double xScale = 10.0 / width, yScale = 10.0 / height;
      //    double heightScale = 3.0;

      // FastNoise::FastNoise myNoise; // Create a FastNoise object
      // myNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type

      // myNoise.SetFrequency(0.05);

      // float heightMap[width][height]; // 2D heightmap to create terrain
      
      // for (int x = 0; x < width; x++)
      // {
      //    for (int y = 0; y < height; y++)
      //    {
      //       heightMap[x][y] = myNoise.GetNoise(x,y);
      //    }
      // }

      // std::vector<std::vector<DDG::Index>> indices; //outside loop
      // std::vector<DDG::Vector> positions; //outside loop
      // int i;
      // for (int row = 0; row < height - 1; ++row) {
      //     for (int col = 0; col < width - 1; ++col) {
            
      //         i += 4;

           
      //         float x1 = col * xScale, x2 = col * xScale, x3 = (col + 1) * xScale, x4 = (col + 1) * xScale;
      //         float y1 = row * yScale, y2 = (row + 1) * yScale, y3 = row * yScale, y4 = (row + 1) * yScale;
      //         float z1 = heightScale*heightMap[row][col], z2 = heightScale*heightMap[row + 1][col], 
      //           z3 = heightScale*heightMap[row][col + 1], z4 = heightScale*heightMap[row + 1][col + 1];

      //          //Create vertex positions vector for Meshdata
      //          DDG::Vector position1;
      //          position1 = DDG::Vector(x1,y1,z1);
      //          DDG::Vector position2;
      //          position2 = DDG::Vector(x2,y2,z2);
      //          DDG::Vector position3; 
      //          position3 = DDG::Vector(x4,y4,z3);
      //          DDG::Vector position4;
      //          position4 = DDG::Vector(x4,y4,z4);

      //          positions.push_back(position1);
      //          positions.push_back(position2);
      //          positions.push_back(position3);
      //          positions.push_back(position4);


      //          //Create Faces for Meshdata   
      //          std::vector<DDG::Index> face1;     
      //          std::vector<DDG::Index> face2;       // two triangles c,b,a and c,a,d 
      //          DDG::Index a;
      //          DDG::Index b;
      //                                                    //      a---d
      //          a = DDG::Index(0+i,0,0);                  //      |\  |
      //          b = DDG::Index(1+i,0,0);                  //      | \ |
      //          DDG::Index c = DDG::Index(2+i,0,0);       //      |  \|
      //          DDG::Index d = DDG::Index(3+i,0,0);       //      b---c

      //          indices.push_back(face1);
      //          indices.push_back(face2);


      //       }
      //    }
      // }
   };
}

#endif
