#pragma once

#include <vector>

#include "../UI/ModelRenderer.h"

class ModelController {
   public:
    ModelController(std::vector<ModelRenderer>& models);
    void ChangeCurrentModel();
    void SetObjectPosition(float x, float y, float z);
    void SetObjectRotation(float x, float y, float z);
    void SetObjectScale(float x, float y, float z);
    void SetLinesColor(float x, float y, float z);
    void SwitchDrawMode();
    s21::Vector3 GetCurrentPosition() const;
    s21::Vector3 GetCurrentRotation() const;
    s21::Vector3 GetCurrentScale() const;
    void ParseTransform(s21::Vector3& position, s21::Vector3 &scale, s21::Vector3 &rotation) {
        m_models[m_currentModelIndex].ParseTransform(position, scale, rotation);
    }
   private:
    std::vector<ModelRenderer>& m_models;
    size_t m_currentModelIndex;
};