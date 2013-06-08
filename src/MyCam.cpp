#include "MyCam.h"

MyCam::MyCam() {
	_initCam = _curCam = CameraPersp(); 
}

MyCam::MyCam(const CameraPersp &inputInitCam) {
	_initCam = _curCam = inputInitCam; 
	updatePickingData();
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
		updatePickingData();
}

void MyCam::updatePickingData() {
	CameraPersp camera = _curCam;

	_viewPlane = (camera.getCenterOfInterestPoint() - camera.getEyePoint()).safeNormalized();
	_viewWidth = _viewPlane.cross(camera.getWorldUp()).safeNormalized();
	_viewHight = _viewWidth.cross(_viewPlane).safeNormalized();


	// convert fovy to radians 
	float rad = camera.getFov() * PI / 180;
	float vLength = tan( rad / 2 ) * camera.getNearClip();
	float hLength = vLength * camera.getAspectRatio();

	_viewHight *= vLength;
	_viewWidth *= hLength;
}
void MyCam::getPickingRay(Vec2f mousePos, Vec3f &rayPos, Vec3f &rayDir) {
	// translate mouse coordinates so that the origin lies in the center
	// of the view port
	mousePos -= (getWindowBounds().getSize() / 2);

	// scale mouse coordinates so that half the view port width and height
	// becomes 1
	mousePos /= (getWindowBounds().getSize() / 2);

	// linear combination to compute intersection of picking ray with
	// view port plane
	rayPos = _curCam.getEyePoint() + _viewPlane*_curCam.getNearClip() + _viewWidth*mousePos.x - _viewHight*mousePos.y;

	// compute direction of picking ray by subtracting intersection point
	// with camera position
	rayDir = rayPos - _curCam.getEyePoint();
}
const CameraPersp& MyCam::getCam() const { 
	return _curCam; 
}
void MyCam::setCam(const CameraPersp &camIn ) { 
	_curCam = camIn; 
	updatePickingData();
}	