/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

#ifndef LinearElasticSpring_h
#define LinearElasticSpring_h

// Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
// Created: 03/19
// Revision: A
//
// Description: This file contains the class definition for LinearElasticSpring.
// LinearElasticSpring is an element defined by one node and by the top half of
// the symmetric stiffness matrix.
// This LinearElasticSpring element will work in 1d, 2d or 3d problems.

#include "Element.h"
#include <Matrix.h>

class Channel;
class UniaxialMaterial;
class Response;

// Type of dimension of element NxDy has dimension x=1,2,3 and
// y=2,4,6,12 degrees-of-freedom for the element
#ifndef ZeroLength_h
    enum Etype { D1N2, D2N4, D2N6, D3N6, D3N12 };
#endif


class LinearElasticSpring : public Element
{
public:
    // constructors
    LinearElasticSpring(int tag, int dimension, int Nd1, int Nd2,
        const ID &direction, const Matrix &stif,
        const Vector y = 0, const Vector x = 0,
        const Vector Mratio = 0, int addRayleigh = 0,
        const Matrix *damp = 0);
    LinearElasticSpring();
    
    // destructor
    ~LinearElasticSpring();
    
    // method to get class type
    const char *getClassType() const {return "LinearElasticSpring";};
    
    // public methods to obtain information about dof & connectivity
    int getNumExternalNodes() const;
    const ID &getExternalNodes();
    Node **getNodePtrs();
    int getNumDOF();
    void setDomain(Domain *theDomain);
    
    // public methods to set the state of the element
    int commitState();
    int revertToLastCommit();
    int revertToStart();
    int update();
    
    // public methods to obtain stiffness,
    // mass, damping and residual information
    const Matrix &getTangentStiff();
    const Matrix &getInitialStiff();
    const Matrix &getDamp();
    
    void zeroLoad();
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);
    
    const Vector &getResistingForce();
    const Vector &getResistingForceIncInertia();
    
    // public methods for element output
    int sendSelf(int commitTag, Channel &theChannel);
    int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);
    int displaySelf(Renderer &, int mode, float fact, const char **displayModes=0, int numModes=0);
    void Print(OPS_Stream &s, int flag = 0);
    
    // public methods for element recorder
    Response *setResponse(const char **argv, int argc, OPS_Stream &s);
    int getResponse(int responseID, Information &eleInfo);

private:
    Etype elemType;
    
    // private methods
    void setUp();
    void setTranGlobalLocal();
    void setTranLocalBasic();
    void addPDeltaForces(Vector &pLocal);
    void addPDeltaStiff(Matrix &kLocal);

    // private attributes - a copy for each object of the class
    int numDIM;                         // 1, 2, or 3 dimensions
    int numDOF;                         // number of dof for LinearElasticSpring
    ID connectedExternalNodes;          // contains the tag of the end node
    Node *theNodes[2];                  // pointer to node
    
    // parameters
    int numDIR;         // number of directions
    ID dir;             // array of directions 0-5
    Matrix kb;          // stiffness matrix in basic system
    Matrix *cb;         // damping matrix in basic system
    Vector x;           // local x direction
    Vector y;           // local y direction
    Vector Mratio;      // p-delta moment distribution ratios
    int addRayleigh;    // flag to add Rayleigh damping
    double L;           // element length
    bool onP0;          // flag to indicate if the element is on P0
    
    Matrix trans;       // transformation matrix for orientation
    Vector ub;          // trial displacements in basic system
    Vector ubdot;       // trial velocities in basic system
    Vector qb;          // measured forces in basic system
    Vector ul;          // displacements in local system
    Matrix Tgl;         // transformation matrix from global to local system
    Matrix Tlb;         // transformation matrix from local to basic system
    
    Matrix *theMatrix;  // pointer to objects matrix (a class wide Matrix)
    Vector *theVector;  // pointer to objects vector (a class wide Vector)
    Vector *theLoad;    // pointer to the load vector
    
    // static data - single copy for all objects of the class
    static Matrix LinearElasticSpringM2;   // class wide matrix for 2*2
    static Matrix LinearElasticSpringM4;   // class wide matrix for 4*4
    static Matrix LinearElasticSpringM6;   // class wide matrix for 6*6
    static Matrix LinearElasticSpringM12;  // class wide matrix for 12*12
    static Vector LinearElasticSpringV2;   // class wide Vector for size 2
    static Vector LinearElasticSpringV4;   // class wide Vector for size 4
    static Vector LinearElasticSpringV6;   // class wide Vector for size 6
    static Vector LinearElasticSpringV12;  // class wide Vector for size 12
};

#endif
