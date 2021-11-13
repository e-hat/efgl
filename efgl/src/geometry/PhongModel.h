#include "efpch.h"

#pragma once

#include "IModel.h"

namespace efgl {
    class PhongModel : public IModel {
    public:
        PhongModel(const std::string& path);
    protected:
        virtual Ref<IMaterial> makeMaterial(aiMaterial* mat) override;
    };
}