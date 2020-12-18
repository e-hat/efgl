#pragma once
#include "efpch.h"

#include "scene/Scene.h"
#include "Shader.h"

#include "platform/OpenGL/ShaderStorageBuffer.h"
#include "platform/OpenGL/UniformBuffer.h"
#include "ComputeShader.h"
#include "texture/Texture.h"
#include "platform/OpenGL/fbo/FrameBuffer.h"

#include "geometry/prim/Quad.h"
#include "util/Time.h"

namespace efgl {
	// Main renderer that does all the heavy lifting.
	// Maybe need more than one of these for different demos,
	// of way of configuring
	class Renderer {
	public:
		Renderer(Ref<Scene> scene);

		void Render();

	private:

		typedef struct GPUPointLight {
			glm::vec4 Position;

			glm::vec4 Ambient;
			glm::vec4 Diffuse;
			glm::vec4 Specular;

			float Constant;
			float Linear;
			float Quadratic;

			float Radius;
		} GPUPointLight;

		// render passes
		void depthPass();
		void colorPass();

		// helpers
		std::vector<GPUPointLight> serializePointLights(const std::vector<PointLight>& pointLights);

		// SHADERS
		Ref<Shader> m_Shader;
		Ref<Shader> m_DepthPassShader;

		// SCENE
		Ref<Scene> m_Scene;

		// SSBOs
		Ref<ShaderStorageBuffer> m_AABBGrid;
		Ref<ShaderStorageBuffer> m_ActiveClusterMask;
		Ref<ShaderStorageBuffer> m_ActiveClusters;
		Ref<ShaderStorageBuffer> m_nActiveClusters;
		Ref<ShaderStorageBuffer> m_Lights;
		Ref<ShaderStorageBuffer> m_LightGrid;
		Ref<ShaderStorageBuffer> m_LightIndices;
		Ref<ShaderStorageBuffer> m_nActiveLights;

		// TEXTURES
		Ref<Texture> m_DepthMap;

		// FRAMEBUFFERS
		Ref<FrameBuffer> m_FBO;

		// UBOs
		Ref<UniformBuffer> m_ClusteringInfo;

		// COMPUTE SHADERS
		Ref<ComputeShader> m_AABBGen;
		Ref<ComputeShader> m_CullClusters;
		Ref<ComputeShader> m_CompressActive;
		Ref<ComputeShader> m_CullLights;

		void makeSSBOs();
		void makeUBOs();
	};
}