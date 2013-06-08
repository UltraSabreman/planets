#include "MyCam.h"

MyCam::MyCam() {
	_initCam = _curCam = CameraPersp(); 
}

MyCam::MyCam(const CameraPersp &inputInitCam) {
	_initCam = _curCam = inputInitCam; 
}

void MyCam::mouseWheel(float wheelSpin) {
	float newCOI = powf(E, -wheelSpin / 20.0f) * _curCam.getCenterOfInterest();
	Vec3f oldTarget = _curCam.getCenterOfInterestPoint();
	Vec3f newEye = oldTarget - _curCam.getViewDirection() * newCOI;
	_curCam.setEyePoint(newEye);
	_curCam.setCenterOfInterest(newCOI);
}

void MyCam::mouseDrag(Vec2f mousePos, bool isLeftDown, bool isRightDown) {
		Vec2f nDeltaPos = (mousePos - _oldMousePos).safeNormalized();

		if(isRightDown) { // panning
			nDeltaPos /= 35; //magic number, bascily snesitivity
			float deltaX = nDeltaPos.x * _curCam.getCenterOfInterest();
			float deltaY = nDeltaPos.y * _curCam.getCenterOfInterest();

			Vec3f mW = _curCam.getViewDirection().normalized();
			Vec3f mU = Vec3f::yAxis().cross(mW).normalized(); //get "up" vector that's perpendicualr to the vir dir and y axis
			Vec3f mV = mW.cross(mU).normalized(); //get "right" vector that's perpendicular to the view dir, and "up" vector
			if (isLeftDown)
				_curCam.setEyePoint(_curCam.getEyePoint() + mU * deltaX + mW * deltaY); //pan left/right + in/out
			else
				_curCam.setEyePoint(_curCam.getEyePoint() + mU * deltaX + mV * deltaY); //pan left/right + up/down
		}
		else if (isLeftDown) { // tumbling
			nDeltaPos /= 15; //magic aswell, same as above.
			float deltaX = -nDeltaPos.x;
			float deltaY = nDeltaPos.y;
			Vec3f mW = _curCam.getViewDirection().normalized();
			bool invertMotion = (_curCam.getOrientation() * Vec3f::yAxis()).y < 0.0f;
			Vec3f mU = Vec3f::yAxis().cross(mW).normalized();

			if( invertMotion ) {
				deltaX = -deltaX;
				deltaY = -deltaY;
			}

			Vec3f rotatedVec = Quatf(mU, deltaY) * (_curCam.getEyePoint() - _curCam.getCenterOfInterestPoint());
			rotatedVec = Quatf(Vec3f::yAxis(), deltaX) * rotatedVec;

			_curCam.setEyePoint(_curCam.getCenterOfInterestPoint() + rotatedVec);
			_curCam.setOrientation(_curCam.getOrientation() * Quatf(mU, deltaY) * Quatf(Vec3f::yAxis(), deltaX));
		}
		_oldMousePos = mousePos;
}

const CameraPersp& MyCam::getCam() const { 
	return _curCam; 
}
void MyCam::setCam(const CameraPersp &camIn ) { 
	_curCam = camIn; 
}	