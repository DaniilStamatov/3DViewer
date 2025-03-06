#include "ModelController.h"
ModelController::ModelController(std::vector<ModelRenderer>& models) : m_models(models), m_currentModelIndex(0) {}

void ModelController::ChangeCurrentModel() { m_currentModelIndex = (m_currentModelIndex + 1) % m_models.size(); }

void ModelController::SetObjectPosition(float x, float y, float z) {
    m_models[m_currentModelIndex].SetObjectPosition(x, y, z);
}

void ModelController::SetObjectRotation(float x, float y, float z) {
    m_models[m_currentModelIndex].SetObjectRotation(x, y, z);
}

void ModelController::SetObjectScale(float x, float y, float z) {
    m_models[m_currentModelIndex].SetObjectScale(x, y, z);
}

void ModelController::SetLinesColor(float x, float y, float z) { m_models[m_currentModelIndex].SetLinesColor(x, y, z); }

void ModelController::SwitchDrawMode() { m_models[m_currentModelIndex].SwitchDrawMode(); }

// s21::Vector3 ModelController::GetCurrentPosition() const { return m_models[m_currentModelIndex].GetPosition(); }

// s21::Vector3 ModelController::GetCurrentRotation() const { return m_models[m_currentModelIndex].GetRotation(); }

// s21::Vector3 ModelController::GetCurrentScale() const { return m_models[m_currentModelIndex].GetScale(); }