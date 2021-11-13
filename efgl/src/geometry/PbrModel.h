#include "efpch.h"

#pragma once

#include "IModel.h"

namespace efgl {
    class PbrModel : IModel {
    public:
        PbrModel(const std::string& path);
    protected:
        virtual Ref<IMaterial> makeMaterial(aiMaterial* mat) override;
    };
}