#include "DefaultSceneLayer.h"

// GLM math library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/common.hpp> // for fmod (floating modulus)

#include <filesystem>

// Graphics
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/Textures/Texture2DArray.h"
#include "Graphics/VertexTypes.h"
#include "Graphics/Font.h"
#include "Graphics/GuiBatcher.h"
#include "Graphics/Framebuffer.h"

// Utilities
#include "Utils/MeshBuilder.h"
#include "Utils/MeshFactory.h"
#include "Utils/ObjLoader.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/FileHelpers.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/StringUtils.h"
#include "Utils/GlmDefines.h"

// Gameplay
#include "Gameplay/Material.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Components/Light.h"

// Components
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Components/Camera.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/MaterialSwapBehaviour.h"
#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/SimpleCameraControl.h"

#include "Gameplay/Components/SimpleObjectControl.h"
#include "Gameplay/Components/SimpleCameraFollow.h"
#include "Gameplay/Components/KillBehaviour.h"
#include "Gameplay/Components/GoalBehaviour.h"
#include "Gameplay/Components/SimpleLightFollow.h"
#include "Gameplay/Components/SimpleToggle.h"
#include "Gameplay/Components/ShaderContainer.h"
#include "Gameplay/Components/SimpleParticleFollow.h"
#include "Gameplay/Components/SunlightMoveBehaviour.h"
#include "Gameplay/Components/EnemyFollowBehaviour.h"

// Physics
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Physics/Colliders/BoxCollider.h"
#include "Gameplay/Physics/Colliders/PlaneCollider.h"
#include "Gameplay/Physics/Colliders/SphereCollider.h"
#include "Gameplay/Physics/Colliders/ConvexMeshCollider.h"
#include "Gameplay/Physics/Colliders/CylinderCollider.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Graphics/DebugDraw.h"

// GUI
#include "Gameplay/Components/GUI/RectTransform.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "Gameplay/InputEngine.h"

#include "Application/Application.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/Texture1D.h"
#include "Application/Layers/ImGuiDebugLayer.h"
#include "Application/Windows/DebugWindow.h"
#include "Gameplay/Components/ShadowCamera.h"
#include "Gameplay/Components/ShipMoveBehaviour.h"

DefaultSceneLayer::DefaultSceneLayer() :
	ApplicationLayer()
{
	Name = "Default Scene";
	Overrides = AppLayerFunctions::OnAppLoad;
}

DefaultSceneLayer::~DefaultSceneLayer() = default;

void DefaultSceneLayer::OnAppLoad(const nlohmann::json& config) {
	_CreateScene();
}

void DefaultSceneLayer::_CreateScene()
{
	using namespace Gameplay;
	using namespace Gameplay::Physics;

	Application& app = Application::Get();

	bool loadScene = false;
	// For now we can use a toggle to generate our scene vs load from file
	if (loadScene && std::filesystem::exists("scene.json")) {
		app.LoadScene("scene.json");
	} else {
		 
		// Basic gbuffer generation with no vertex manipulation
		ShaderProgram::Sptr deferredForward = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});
		deferredForward->SetDebugName("Deferred - GBuffer Generation");  

		// Our foliage shader which manipulates the vertices of the mesh
		ShaderProgram::Sptr foliageShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/foliage.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});  
		foliageShader->SetDebugName("Foliage");   

		// This shader handles our multitexturing example
		ShaderProgram::Sptr multiTextureShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/vert_multitextured.glsl" },  
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_multitextured.glsl" }
		});
		multiTextureShader->SetDebugName("Multitexturing"); 

		// This shader handles our displacement mapping example
		ShaderProgram::Sptr displacementShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});
		displacementShader->SetDebugName("Displacement Mapping");

		// This shader handles our cel shading example
		ShaderProgram::Sptr celShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/cel_shader.glsl" }
		});
		celShader->SetDebugName("Cel Shader");

		ShaderProgram::Sptr blinnPhong = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_blinn_phong_textured.glsl" }
		});
		blinnPhong->SetDebugName("Blinn Phong Shader");
		

		

		Texture2DArray::Sptr particleTex = ResourceManager::CreateAsset<Texture2DArray>("textures/particles.png", 2, 2);

		//DebugWindow::Sptr debugWindow = app.GetLayer<ImGuiDebugLayer>()->GetWindow<DebugWindow>();

#pragma region Basic Texture Creation
		Texture2DDescription singlePixelDescriptor;
		singlePixelDescriptor.Width = singlePixelDescriptor.Height = 1;
		singlePixelDescriptor.Format = InternalFormat::RGB8;

		float normalMapDefaultData[3] = { 0.5f, 0.5f, 1.0f };
		Texture2D::Sptr normalMapDefault = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		normalMapDefault->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, normalMapDefaultData);

		float solidGrey[3] = { 0.5f, 0.5f, 0.5f };
		float solidBlack[3] = { 0.0f, 0.0f, 0.0f };
		float solidWhite[3] = { 1.0f, 1.0f, 1.0f };

		Texture2D::Sptr solidBlackTex = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		solidBlackTex->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, solidBlack);

		Texture2D::Sptr solidGreyTex = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		solidGreyTex->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, solidGrey);

		Texture2D::Sptr solidWhiteTex = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		solidWhiteTex->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, solidWhite);

#pragma endregion 

		// Loading in a 1D LUT
		Texture1D::Sptr toonLut = ResourceManager::CreateAsset<Texture1D>("luts/toon-1D.png"); 
		toonLut->SetWrap(WrapMode::ClampToEdge);

		// Here we'll load in the cubemap, as well as a special shader to handle drawing the skybox
		TextureCube::Sptr testCubemap = ResourceManager::CreateAsset<TextureCube>("cubemaps/ocean/ocean.jpg");
		ShaderProgram::Sptr      skyboxShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/skybox_vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/skybox_frag.glsl" } 
		});
		  
		// Create an empty scene
		Scene::Sptr scene = std::make_shared<Scene>();  

		// Setting up our enviroment map
		scene->SetSkyboxTexture(testCubemap); 
		scene->SetSkyboxShader(skyboxShader);
		// Since the skybox I used was for Y-up, we need to rotate it 90 deg around the X-axis to convert it to z-up 
		scene->SetSkyboxRotation(glm::rotate(MAT4_IDENTITY, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));

		// Loading in a color lookup table
		Texture3D::Sptr lut = ResourceManager::CreateAsset<Texture3D>("luts/cool.CUBE");   

		// Configure the color correction LUT
		scene->SetColorLUT(lut);

		// Create our materials
		// This will be our box material, with no environment reflections
		

		// This will be the reflective material, we'll make the whole thing 90% reflective
	

		// Our toon shader material
		/*
		Material::Sptr toonMaterial = ResourceManager::CreateAsset<Material>(celShader);
		{
			toonMaterial->Name = "Toon"; 
			toonMaterial->Set("u_Material.AlbedoMap", boxTexture);
			toonMaterial->Set("u_Material.NormalMap", normalMapDefault);
			toonMaterial->Set("s_ToonTerm", toonLut);
			toonMaterial->Set("u_Material.Shininess", 0.1f); 
			toonMaterial->Set("u_Material.Steps", 8);
		}*/



		

		


		// Create some lights for our scene
		GameObject::Sptr lightParent = scene->CreateGameObject("Lights");

		for (int ix = 0; ix < 50; ix++) {
			GameObject::Sptr light = scene->CreateGameObject("Light");
			light->SetPostion(glm::vec3(glm::diskRand(25.0f), 1.0f));
			lightParent->AddChild(light);

			Light::Sptr lightComponent = light->Add<Light>();
			lightComponent->SetColor(glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f)));
			lightComponent->SetRadius(glm::linearRand(0.1f, 10.0f));
			lightComponent->SetIntensity(glm::linearRand(1.0f, 2.0f));
			SimpleToggle::Sptr toggle = light->Add<SimpleToggle>();
			toggle->isAmbLight = true;
		}

		// We'll create a mesh that is a simple plane that we can resize later
		MeshResource::Sptr planeMesh = ResourceManager::CreateAsset<MeshResource>();
		planeMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(1.0f)));
		planeMesh->GenerateMesh();

		MeshResource::Sptr sphere = ResourceManager::CreateAsset<MeshResource>();
		sphere->AddParam(MeshBuilderParam::CreateIcoSphere(ZERO, ONE, 5));
		sphere->GenerateMesh();

		// Set up the scene's camera
		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion({ -3, -1, 5 });
			camera->LookAt(glm::vec3(0.0f));

			camera->Add<SimpleCameraFollow>();

			//camera->Add<SimpleCameraControl>();

			// This is now handled by scene itself!
			//Camera::Sptr cam = camera->Add<Camera>();
			// Make sure that the camera is set as the scene's main camera!
			//scene->MainCamera = cam;
		}

		GameObject::Sptr nvToggle = scene->CreateGameObject("nvToggle");
		{
			SimpleToggle::Sptr toggle = nvToggle->Add<SimpleToggle>();
			toggle->nvToggleTime = 0.0f;
		}


		// Set up all our sample objects
		GameObject::Sptr plane = scene->CreateGameObject("Plane");
		{
			Texture2D::Sptr rock_tex = ResourceManager::CreateAsset<Texture2D>("objs/terrain/rock_tex.png");
			Texture2D::Sptr rock_spec_tex = ResourceManager::CreateAsset<Texture2D>("objs/terrain/rock_spec_tex.png");


			Material::Sptr plane_Material = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				plane_Material->Name = "Plane Material";
				plane_Material->Set("u_Material.AlbedoMap", rock_tex);
				plane_Material->Set("u_Material.Specular", rock_spec_tex);
				plane_Material->Set("u_Material.Shininess", 0.8f);
				plane_Material->Set("u_Material.NormalMap", normalMapDefault);
			}

			Material::Sptr plane_Material_blinn = ResourceManager::CreateAsset<Material>(blinnPhong);
			{
				plane_Material_blinn->Name = "Plane Material No Specular";
				plane_Material_blinn->Set("u_Material.Diffuse", rock_tex);
				plane_Material_blinn->Set("u_Material.Shininess", 0.8f);
			}

			Material::Sptr plane_Material_noDiff = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				plane_Material_noDiff->Name = "Plane Material No Diffuse";
				plane_Material_noDiff->Set("u_Material.AlbedoMap", solidWhiteTex);
				plane_Material_noDiff->Set("u_Material.Shininess", 0.8f);
			}

			MeshResource::Sptr tiledMesh = ResourceManager::CreateAsset<MeshResource>();
			tiledMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(30.0f), glm::vec2(10.0f)));
			tiledMesh->GenerateMesh();

			RenderComponent::Sptr renderer = plane->Add<RenderComponent>();
			renderer->SetMesh(tiledMesh);
			renderer->SetMaterial(plane_Material);

			RigidBody::Sptr physics = plane->Add<RigidBody>();
			physics->AddCollider(BoxCollider::Create(glm::vec3(50.0f, 50.0f, 1.0f)))->SetPosition({ 0,0,-1 });

			SimpleToggle::Sptr toggle = plane->Add<SimpleToggle>();
			toggle->addMatrial(plane_Material, "ORIG");
			toggle->addMatrial(plane_Material_blinn, "SPEC");
			toggle->addMatrial(plane_Material_noDiff, "DIFF");
		}

		// Add some walls :3
		{
			MeshResource::Sptr wall = ResourceManager::CreateAsset<MeshResource>();
			wall->AddParam(MeshBuilderParam::CreateCube(ZERO, ONE));
			wall->GenerateMesh();

			Texture2D::Sptr brick_tex = ResourceManager::CreateAsset<Texture2D>("textures/displacement_map.png");
			Texture2D::Sptr brick_norm_tex = ResourceManager::CreateAsset<Texture2D>("textures/normal_map.png");

			Material::Sptr whiteBrick = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				whiteBrick->Name = "White Bricks";
				whiteBrick->Set("u_Material.AlbedoMap", brick_tex);
				whiteBrick->Set("u_Material.Specular", solidGrey);
				whiteBrick->Set("u_Material.NormalMap", brick_norm_tex);
			}

			Material::Sptr whiteBrick_blinn = ResourceManager::CreateAsset<Material>(blinnPhong);
			{
				whiteBrick_blinn->Name = "White Bricks No Specular";
				whiteBrick_blinn->Set("u_Material.Diffuse", brick_tex);
				whiteBrick_blinn->Set("u_Material.Shininess", 0.8f);
			}

			Material::Sptr whiteBrick_noDiff = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				whiteBrick_noDiff->Name = "White Bricks No Diffuse";
				whiteBrick_noDiff->Set("u_Material.AlbedoMap", solidWhiteTex);
				whiteBrick_noDiff->Set("u_Material.Shininess", 0.8f);
			}

			GameObject::Sptr wall1 = scene->CreateGameObject("Wall1"); 
			{
				wall1->Add<RenderComponent>()->SetMesh(wall)->SetMaterial(whiteBrick);
				wall1->SetScale(glm::vec3(20.0f, 1.0f, 3.0f));
				wall1->SetPostion(glm::vec3(0.0f, 10.0f, 1.5f));
				wall1->Add<RigidBody>()->AddCollider(BoxCollider::Create())->SetScale(glm::vec3(10.0f, 0.5f, 1.5f));
				SimpleToggle::Sptr toggle = wall1->Add<SimpleToggle>();
				toggle->addMatrial(whiteBrick, "ORIG");
				toggle->addMatrial(whiteBrick_blinn, "SPEC");
				toggle->addMatrial(whiteBrick_noDiff, "DIFF");
			}
			

			

			GameObject::Sptr wall2 = scene->CreateGameObject("Wall2");
			{
				wall2->Add<RenderComponent>()->SetMesh(wall)->SetMaterial(whiteBrick);
				wall2->SetScale(glm::vec3(20.0f, 1.0f, 3.0f));
				wall2->SetPostion(glm::vec3(0.0f, -10.0f, 1.5f));
				wall2->Add<RigidBody>()->AddCollider(BoxCollider::Create())->SetScale(glm::vec3(10.0f, 0.5f, 1.5f));
				SimpleToggle::Sptr toggle = wall2->Add<SimpleToggle>();
				toggle->addMatrial(whiteBrick, "ORIG");
				toggle->addMatrial(whiteBrick_blinn, "SPEC");
				toggle->addMatrial(whiteBrick_noDiff, "DIFF");
			}
			
			

			GameObject::Sptr wall3 = scene->CreateGameObject("Wall3");
			{
				wall3->Add<RenderComponent>()->SetMesh(wall)->SetMaterial(whiteBrick);
				wall3->SetScale(glm::vec3(1.0f, 20.0f, 3.0f));
				wall3->SetPostion(glm::vec3(10.0f, 0.0f, 1.5f));
				wall3->Add<RigidBody>()->AddCollider(BoxCollider::Create())->SetScale(glm::vec3(0.5f, 10.0f, 1.5f));
				SimpleToggle::Sptr toggle = wall3->Add<SimpleToggle>();
				toggle->addMatrial(whiteBrick, "ORIG");
				toggle->addMatrial(whiteBrick_blinn, "SPEC");
				toggle->addMatrial(whiteBrick_noDiff, "DIFF");
			}
			
			

			GameObject::Sptr wall4 = scene->CreateGameObject("Wall4");
			{
				wall4->Add<RenderComponent>()->SetMesh(wall)->SetMaterial(whiteBrick);
				wall4->SetScale(glm::vec3(1.0f, 20.0f, 3.0f));
				wall4->SetPostion(glm::vec3(-10.0f, 0.0f, 1.5f));
				wall4->Add<RigidBody>()->AddCollider(BoxCollider::Create())->SetScale(glm::vec3(0.5f, 10.0f, 1.5f));
				SimpleToggle::Sptr toggle = wall4->Add<SimpleToggle>();
				toggle->addMatrial(whiteBrick, "ORIG");
				toggle->addMatrial(whiteBrick_blinn, "SPEC");
				toggle->addMatrial(whiteBrick_noDiff, "DIFF");
			}
			
			
		}

		GameObject::Sptr zelda = scene->CreateGameObject("Zelda");
		{
			Material::Sptr zeldaMat = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				zeldaMat->Name = "Zelda Material";
				zeldaMat->Set("u_Material.AlbedoMap", ResourceManager::CreateAsset<Texture2D>("objs/zelda/zelda_tex.png"));
				//polka->Set("u_Material.Specular", solidBlackTex);
				zeldaMat->Set("u_Material.NormalMap", normalMapDefault);
				//polka->Set("u_Material.EmissiveMap", ResourceManager::CreateAsset<Texture2D>("textures/polka.png"));
			}

			Material::Sptr zeldaMat_noDiff = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				zeldaMat_noDiff->Name = "Zelda Material No Diffuse";
				zeldaMat_noDiff->Set("u_Material.AlbedoMap", solidWhiteTex);
				zeldaMat_noDiff->Set("u_Material.Shininess", 0.8f);
			}

			zelda->SetScale(glm::vec3(0.1, 0.1, 0.1));

			RenderComponent::Sptr renderer = zelda->Add<RenderComponent>();
			renderer->SetMesh(ResourceManager::CreateAsset<MeshResource>("objs/zelda/zelda.obj"));
			renderer->SetMaterial(zeldaMat);
			 
			RigidBody::Sptr physics = zelda->Add<RigidBody>(RigidBodyType::Dynamic);
			ICollider::Sptr collider = physics->AddCollider(SphereCollider::Create(0.4f));
			
			 
			zelda->Add<JumpBehaviour>();

			SimpleObjectControl::Sptr control = zelda->Add<SimpleObjectControl>();
			control->setCamera(camera);

			camera->Get<SimpleCameraFollow>()->setFollowObj(zelda);

			Light::Sptr light = zelda->Add<Light>();
			light->IsEnabled = false;

			SimpleToggle::Sptr toggle = zelda->Add<SimpleToggle>();
			toggle->addMatrial(zeldaMat, "ORIG");
			toggle->addMatrial(zeldaMat_noDiff, "DIFF");

			ParticleSystem::Sptr particleManager = zelda->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(-1.0f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 3;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			emitter.Lifetime = 1.0f / 50.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 0.5f;
			emitter.SphereEmitterData.SizeRange = { 0.5f, 1.0f };

			particleManager->AddEmitter(emitter);
		}

		MeshResource::Sptr mushroom_mesh = ResourceManager::CreateAsset<MeshResource>("objs/mushroom/mushroom.obj");
		GameObject::Sptr mushroom_goal_obj = scene->CreateGameObject("mushroom to goal");
		{
			mushroom_goal_obj->SetPostion(glm::vec3(4.0f, 5.0f, 0.0f));
			mushroom_goal_obj->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
			mushroom_goal_obj->SetRotation(glm::vec3(0.0f, 0.0f, -145.0f));

			Material::Sptr mushroom_goal_Material = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				mushroom_goal_Material->Name = "Goal Mushroom Material";
				mushroom_goal_Material->Set("u_Material.AlbedoMap", ResourceManager::CreateAsset<Texture2D>("objs/mushroom/mushroom_goal_tex.png"));
				mushroom_goal_Material->Set("u_Material.Specular", ResourceManager::CreateAsset<Texture2D>("objs/mushroom/mushroom_spec_tex.png"));
				mushroom_goal_Material->Set("u_Material.NormalMap", ResourceManager::CreateAsset<Texture2D>("objs/mushroom/textures/DefaultMaterial_normal.png"));
			}

			Material::Sptr mushroom_goal_Material_blinn = ResourceManager::CreateAsset<Material>(blinnPhong);
			{
				mushroom_goal_Material_blinn->Name = "Goal Mushroom Material No Specular";
				mushroom_goal_Material_blinn->Set("u_Material.Diffuse", ResourceManager::CreateAsset<Texture2D>("objs/mushroom/mushroom_goal_tex.png"));
				mushroom_goal_Material_blinn->Set("u_Material.Shininess", 0.8f);
			}

			Material::Sptr mushroom_goal_Material_noDiff = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				mushroom_goal_Material_noDiff->Name = "Goal Mushroom Material No Diffuse";
				mushroom_goal_Material_noDiff->Set("u_Material.AlbedoMap", solidWhiteTex);
				mushroom_goal_Material_noDiff->Set("u_Material.Shininess", 0.8f);
			}

			RenderComponent::Sptr renderer = mushroom_goal_obj->Add<RenderComponent>();
			renderer->SetMesh(mushroom_mesh);
			renderer->SetMaterial(mushroom_goal_Material);

			

			TriggerVolume::Sptr volume = mushroom_goal_obj->Add<TriggerVolume>();
			volume->AddCollider(ConvexMeshCollider::Create());
			volume->AddComponent<GoalBehaviour>();

			SimpleToggle::Sptr toggle = mushroom_goal_obj->Add<SimpleToggle>();
			toggle->addMatrial(mushroom_goal_Material, "ORIG");
			toggle->addMatrial(mushroom_goal_Material_blinn, "SPEC");
			toggle->addMatrial(mushroom_goal_Material_noDiff, "DIFF");
		}

		GameObject::Sptr mushroom_kill_obj = scene->CreateGameObject("mushroom to death");
		{
			mushroom_kill_obj->SetPostion(glm::vec3(-7.0f, -6.0f, 0.0f));
			mushroom_kill_obj->SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
			mushroom_kill_obj->SetRotation(glm::vec3(0.0f, 0.0f, 36.0f));


			Material::Sptr mushroom_kill_Material = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				mushroom_kill_Material->Name = "Kill Mushroom Material";
				mushroom_kill_Material->Set("u_Material.AlbedoMap", ResourceManager::CreateAsset<Texture2D>("objs/mushroom/mushroom_kill_tex.png"));
				//bulletMat->Set("u_Material.Specular", solidBlackTex);
				mushroom_kill_Material->Set("u_Material.NormalMap", normalMapDefault);
				//bulletMat->Set("u_Material.EmissiveMap", ResourceManager::CreateAsset<Texture2D>("textures/polka.png"));
			}

			Material::Sptr mushroom_kill_Material_blinn = ResourceManager::CreateAsset<Material>(blinnPhong);
			{
				mushroom_kill_Material_blinn->Name = "Kill Mushroom Material No Specular";
				mushroom_kill_Material_blinn->Set("u_Material.Diffuse", ResourceManager::CreateAsset<Texture2D>("objs/mushroom/mushroom_kill_tex.png"));
				mushroom_kill_Material_blinn->Set("u_Material.Shininess", 0.8f);
			}

			Material::Sptr mushroom_kill_Material_noDiff = ResourceManager::CreateAsset<Material>(deferredForward);
			{
				mushroom_kill_Material_noDiff->Name = "Kill Mushroom Material No Diffuse";
				mushroom_kill_Material_noDiff->Set("u_Material.AlbedoMap", solidWhiteTex);
				mushroom_kill_Material_noDiff->Set("u_Material.Shininess", 0.8f);
			}

			RenderComponent::Sptr renderer = mushroom_kill_obj->Add<RenderComponent>();
			renderer->SetMesh(mushroom_mesh);
			renderer->SetMaterial(mushroom_kill_Material);

			mushroom_kill_obj->Add<EnemyFollowBehaviour>();

			TriggerVolume::Sptr volume = mushroom_kill_obj->Add<TriggerVolume>();
			volume->AddCollider(ConvexMeshCollider::Create());
			volume->AddComponent<KillBehaviour>();

			SimpleToggle::Sptr toggle = mushroom_kill_obj->Add<SimpleToggle>();
			toggle->addMatrial(mushroom_kill_Material, "ORIG");
			toggle->addMatrial(mushroom_kill_Material_blinn, "SPEC");
			toggle->addMatrial(mushroom_kill_Material_noDiff, "DIFF");
		}

		GameObject::Sptr shadowCaster = scene->CreateGameObject("Shadow Light");
		{
			shadowCaster->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

			// Set position in the scene
			shadowCaster->SetPostion(glm::vec3(4.0f, 4.0f, 80.0f));
			shadowCaster->LookAt(glm::vec3(0.0f));

			// Create and attach a renderer for the monkey
			ShadowCamera::Sptr shadowCam = shadowCaster->Add<ShadowCamera>();
			shadowCam->SetProjection(glm::perspective(glm::radians(120.0f), 1.0f, 0.1f, 100.0f));
			shadowCam->Bias = 0.0f;
			shadowCam->NormalBias = 0.00001f;
			shadowCam->SetBufferResolution(glm::ivec2(8192, 8192));

			shadowCaster->Add<SunlightMoveBehaviour>();

			shadowCaster->Add<SimpleToggle>();
		} 
		
		//GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		//GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("scene-manifest.json");
		// Save the scene to a JSON file
		scene->Save("scene.json");

		// Send the scene to the application
		app.LoadScene(scene);
	}
}
