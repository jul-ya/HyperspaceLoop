#pragma once

#include "Model.h"
#include "Transform.h"

class GameObject {
public:

	GameObject(Model model, Transform transform) : model(model), transform(transform) {}

	~GameObject() {}

	Model getModel() {
		return model;
	}

	Transform getTransform() {
		return transform;
	}

private:

	Model model;
	Transform transform;
};
