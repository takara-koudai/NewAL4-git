#include "Ground.h"
#include <assert.h>

void Ground::Initialize(Model* model) 
{ 
	assert(model);

	GroundModel_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};

	worldTransform_.translation_.z = 10.0f;
}

void Ground::Update() 
{ 

	worldTransform_.UpdateMatrix();

}

void Ground::Draw(const ViewProjection& viewProjection) 
{
	GroundModel_->Draw(worldTransform_, viewProjection);
}
