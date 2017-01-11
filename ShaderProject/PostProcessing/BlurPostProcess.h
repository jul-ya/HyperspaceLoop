#pragma once

#include <iostream>
#include "PostProcessing.h"
#include "..\FBuffer.h"
#include "..\Shader.h"


class BlurPostProcessing : public PostProcessing {

public:

	virtual void setup() {

	}

	virtual void execute(FBuffer frameBuffer) {
		std::cout << "wörks" << std::endl;
	}



private:

	



};