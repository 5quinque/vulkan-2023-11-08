#include "Box.hpp"

int Box::totalNbBoxes = 0;

// Constructor
// Box::Box(glm::vec3 size, bool createRigidBody) : Model(totalNbBoxes, size,
// createRigidBody) {
Box::Box() : Model(totalNbBoxes) {
    MODEL_PATH = "models/cube.obj";
    TEXTURE_PATH = "textures/the_metamorphosis.jpg";

    // // Create the collision shape for the rigid body (box shape)
    // // ReactPhysics3D will clone this object to create an internal one.
    // Therefore,
    // // it is OK if this object is destroyed right after calling
    // RigidBody::addCollisionShape() mBoxShape =
    // mPhysicsCommon.createBoxShape(rp3d::Vector3(mSize[0], mSize[1],
    // mSize[2]));

    // mPreviousTransform = rp3d::Transform::identity();

    // if (createRigidBody) {

    //     // Create a rigid body in the physics world
    //     rp3d::RigidBody* body = world->createRigidBody(mPreviousTransform);
    //     mCollider = body->addCollider(mBoxShape,
    //     rp3d::Transform::identity());
    //     body->updateMassPropertiesFromColliders();
    //     mBody = body;
    // }
    // else {

    //     // Create a body in the physics world
    //     mBody = world->createCollisionBody(mPreviousTransform);
    //     mCollider = mBody->addCollider(mBoxShape,
    //     rp3d::Transform::identity());
    // }

    totalNbBoxes++;
}

// void Box::setModelMatrix(glm::mat4 model) {
//     this->model = model;

//     // // Update the scaling matrix
//     // std::cout << "Box::setModelMatrix(), model: " << std::endl;
//     // std::cout << model[0][0] << " " << model[0][1] << " " << model[0][2]
//     << " " << model[0][3] << std::endl;
//     // std::cout << model[1][0] << " " << model[1][1] << " " << model[1][2]
//     << " " << model[1][3] << std::endl;
//     // std::cout << model[2][0] << " " << model[2][1] << " " << model[2][2]
//     << " " << model[2][3] << std::endl;
//     // std::cout << model[3][0] << " " << model[3][1] << " " << model[3][2]
//     << " " << model[3][3] << std::endl;
//     // mScalingMatrix = glm::mat4(mSize[0], 0, 0, 0,
//     //                             0, mSize[1], 0, 0,
//     //                             0, 0, mSize[2], 0,
//     //                             0, 0, 0, 1);
//     // std::cout << "Box::setModelMatrix(), mScalingMatrix: " << std::endl;
//     // std::cout << mScalingMatrix[0][0] << " " << mScalingMatrix[0][1] << "
//     " << mScalingMatrix[0][2] << " " << mScalingMatrix[0][3] << std::endl;
//     // std::cout << mScalingMatrix[1][0] << " " << mScalingMatrix[1][1] << "
//     " << mScalingMatrix[1][2] << " " << mScalingMatrix[1][3] << std::endl;
//     // std::cout << mScalingMatrix[2][0] << " " << mScalingMatrix[2][1] << "
//     " << mScalingMatrix[2][2] << " " << mScalingMatrix[2][3] << std::endl;
//     // std::cout << mScalingMatrix[3][0] << " " << mScalingMatrix[3][1] << "
//     " << mScalingMatrix[3][2] << " " << mScalingMatrix[3][3] << std::endl;

//     // this->model = model * mScalingMatrix;
// }
// void Box::setModelMatrix(glm::mat4 model) {
//     this->model = model;
// }