/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <cmath>

CamtransCamera::CamtransCamera()
{
    m_near = 1.f;
    m_far = 30.f;
    m_aspectRatio = 1.f;
    setHeightAngle(60.f);
    m_eye = glm::vec4(2.f, 2.f, 2.f, 1.f);
    m_up = glm::vec4(0.f, 1.f, 0.f, 0.f);
    orientLook(m_eye, glm::vec4(-1, -1, -1, 0), m_up);
}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    setHeightAngle(glm::degrees(m_thetaH));
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    return m_perspectiveTransform * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransform;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    return m_up;
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;
    m_w = -glm::normalize(look);
    m_w[3] = 0.f;
    m_v = glm::normalize(m_up - (glm::dot(m_up, m_w) * m_w));
    m_v[3] = 0.f;
    m_u = glm::vec4(glm::cross(glm::vec3(m_v[0], m_v[1], m_v[2]),
            glm::vec3(m_w[0], m_w[1], m_w[2])), 0.f);
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    m_thetaH = glm::radians(h);
    float height = 2.f * m_far * tan(m_thetaH / 2.f);
    m_thetaW = 2.f * atan((height * m_aspectRatio) / (2.f * m_far));
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye = m_eye + v;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    float radTheta = glm::radians(degrees);
    glm::vec4 v0 = m_v;
    glm::vec4 w0 = m_w;
    m_v = w0 * static_cast<float>(sin(radTheta))
            + v0 * static_cast<float>(cos(radTheta));
    m_w = w0 * static_cast<float>(cos(radTheta))
            - v0 * static_cast<float>(sin(radTheta));
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    float radTheta = glm::radians(degrees);
    glm::vec4 u0 = m_u;
    glm::vec4 w0 = m_w;
    m_u = u0 * static_cast<float>(cos(radTheta))
            - w0 * static_cast<float>(sin(radTheta));
    m_w = u0 * static_cast<float>(sin(radTheta))
            + w0 * static_cast<float>(cos(radTheta));
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    float radTheta = glm::radians(-degrees);
    glm::vec4 u0 = m_u;
    glm::vec4 v0 = m_v;
    m_u = u0 * static_cast<float>(cos(radTheta))
            - v0 * static_cast<float>(sin(radTheta));
    m_v = u0 * static_cast<float>(sin(radTheta))
            + v0 * static_cast<float>(cos(radTheta));
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}


void CamtransCamera::updateProjectionMatrix() {
    updatePerspectiveMatrix();
    updateScaleMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {
    float c = -m_near/m_far;
    m_perspectiveTransform = glm::transpose(glm::mat4x4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, -1.f/(c+1.f), c/(c+1.f),
            0.f, 0.f, -1.f, 0.f
    ));
}

void CamtransCamera::updateScaleMatrix() {
    float cotW = 1.f / tan(m_thetaW / 2.f);
    float cotH = 1.f / tan(m_thetaH / 2.f);
    m_scaleMatrix = glm::transpose(glm::mat4x4(
            cotW / m_far, 0.f, 0.f, 0.f,
            0.f, cotH / m_far, 0.f, 0.f,
            0.f, 0.f, 1.f / m_far, 0.f,
            0.f, 0.f, 0.f, 1.f
    ));
}

void CamtransCamera::updateViewMatrix() {
    updateTranslationMatrix();
    updateRotationMatrix();
}

void CamtransCamera::updateRotationMatrix() {
    m_rotationMatrix = glm::transpose(glm::mat4x4(
            m_u[0], m_u[1], m_u[2], 0.f,
            m_v[0], m_v[1], m_v[2], 0.f,
            m_w[0], m_w[1], m_w[2], 0.f,
            0.f, 0.f, 0.f, 1.f
    ));
}

void CamtransCamera::updateTranslationMatrix() {
    m_translationMatrix = glm::transpose(glm::mat4x4(
            1.f, 0.f, 0.f, -m_eye[0],
            0.f, 1.f, 0.f, -m_eye[1],
            0.f, 0.f, 1.f, -m_eye[2],
            0.f, 0.f, 0.f, 1.f
    ));
}
