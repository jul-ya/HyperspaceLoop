#pragma once


#include "..\FBuffer.h"

class PostProcessing {
public:

	virtual void setup() = 0;
	virtual void execute(FBuffer frameBuffer) = 0;
	virtual FBuffer* getOutputBuffer() {
		return frameBuffer;
	}

private:
	FBuffer* frameBuffer;


};