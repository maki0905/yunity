#include "BlockManager.h"

#include "ImGuiManager.h"

void BlockManager::Initialize(Camera* camera, World* world)
{
	blocks_.clear();
	camera_ = camera;
	file_ = CSV::GetInstance()->LoadCSV("Block");
	world_ = world;

	std::string line;
	Block* block = new Block();
	block->Initialize(camera_);

	while (getline(*file_, line)) {
		std::istringstream line_stream(line);

		std::string word;
		getline(line_stream, word, ',');


		if (word.find("Scale") == 0) {
			Vector3 scale{};

			getline(line_stream, word, ',');
			scale.x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			scale.y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			scale.z = (float)std::atof(word.c_str());

			block->SetScale(scale);
		}

		if (word.find("Rotation") == 0) {
			Vector3 rotation{};

			getline(line_stream, word, ',');
			rotation.x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			rotation.y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			rotation.z = (float)std::atof(word.c_str());

			block->SetRotation(rotation);
		}

		if (word.find("Translate") == 0) {
			Vector3 translate{};

			getline(line_stream, word, ',');
			translate.x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			translate.y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			translate.z = (float)std::atof(word.c_str());

			block->SetTranslate(translate);

			blocks_.push_back(block);

			block = new Block();
			block->Initialize(camera_);
		}
	}

	for (auto b : blocks_) {
		world->Add(b);
	}

}

void BlockManager::Update(World* world)
{
	uint32_t count = 0;
	ImGui::Begin("Blocks");

	//for (std::list<Block*>::iterator blockIterator = blocks_.begin(); blockIterator != blocks_.end();) {
	//	if (ImGui::TreeNode((std::to_string(count)).c_str())) {
	//		Transform transform{};
	//		transform.scale = (*blockIterator)->GetWordTransform().scale_;
	//		transform.rotate = (*blockIterator)->GetWordTransform().rotation_;
	//		transform.translate = (*blockIterator)->GetWordTransform().translation_;

	//		ImGui::SliderFloat3("Scale", &transform.scale.x, 0.0f, 100.0f);
	//		ImGui::SliderFloat3("Rotation", &transform.rotate.x, 0.0f, 3.14f);
	//		ImGui::SliderFloat3("Translate", &transform.translate.x, -100.0f, 100.0f);

	//		(*blockIterator)->SetScale(transform.scale);
	//		(*blockIterator)->SetRotation(transform.rotate);
	//		(*blockIterator)->SetTranslate(transform.translate);

	//		if (ImGui::Button("Delete")) {
	//			blockIterator = blocks_.erase(blockIterator);
	//			continue;
	//		}
	//		

	//		ImGui::TreePop();
	//	}
	//	(*blockIterator)->Update();

	//	count++;

	//	// 次のイテレーターへ
	//	++blockIterator;
	//}

	std::vector<int> num;

	for (auto& block : blocks_) {
		if (ImGui::TreeNode((std::to_string(count)).c_str())) {
			EulerTransform transform{};
			transform.scale = block->GetWordTransform().scale_;
			transform.rotate = block->GetWordTransform().rotation_;
			transform.translate = block->GetWordTransform().translation_;

			ImGui::SliderFloat3("Scale", &transform.scale.x, 0.0f, 100.0f);
			ImGui::SliderFloat3("Rotation", &transform.rotate.x, 0.0f, 3.14f);
			ImGui::SliderFloat3("Translate", &transform.translate.x, -100.0f, 100.0f);

			block->SetScale(transform.scale);
			block->SetRotation(transform.rotate);
			block->SetTranslate(transform.translate);

			if (ImGui::Button("Delete")) {
				num.push_back(count);
			}

			ImGui::TreePop();
		}
		count++;
		block->Update();
	}

	if (ImGui::Button("Add")) {
		Add(world);
		
	}
	if (ImGui::Button("Save")) {
		Save();
	}
	ImGui::End();

	for (auto& n : num) {
		blocks_.erase(blocks_.begin() + n);
		Save();
	}
	

}

void BlockManager::Draw()
{
	for (auto& block : blocks_) {
		block->Draw();
	}
}

void BlockManager::Add(World* world)
{
	Block* block = new Block();
	block->Initialize(camera_);
	world->Add(block);
	blocks_.push_back(block);

}

void BlockManager::Save()
{
	std::ofstream outputFile("Resources/CSV/Block.csv");
	for (auto& block : blocks_) {
		outputFile << "Scale,";
		outputFile << block->GetWordTransform().scale_.x;
		outputFile << ',';
		outputFile << block->GetWordTransform().scale_.y;
		outputFile << ',';
		outputFile << block->GetWordTransform().scale_.z;
		outputFile << '\n';
		outputFile << "Rotation,";
		outputFile << block->GetWordTransform().rotation_.x;
		outputFile << ',';
		outputFile << block->GetWordTransform().rotation_.y;
		outputFile << ',';
		outputFile << block->GetWordTransform().rotation_.z;
		outputFile << '\n';
		outputFile << "Translate,";
		outputFile << block->GetWordTransform().translation_.x;
		outputFile << ',';
		outputFile << block->GetWordTransform().translation_.y;
		outputFile << ',';
		outputFile << block->GetWordTransform().translation_.z;
		outputFile << '\n';
	}
	outputFile.close();

}


