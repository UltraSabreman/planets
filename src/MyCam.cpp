#include "MyCam.h"

MyCam::MyCam() {
	_initCam = _curCam = CameraPersp(); 
}

MyCam::MyCam(const CameraPersp &inputInitCam) {
	_initCam = _curCam = inputInitCam; 
	update();
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

	_viewDir = (camera.getCenterOfInterestPoint() - camera.getEyePoint()).safeNormalized();
	_viewWidth = _viewDir.cross(camera.getWorldUp()).safeNormalized();
	_viewHight = _viewWidth.cross(_viewDir).safeNormalized();


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
	rayPos = _curCam.getEyePoint() + _viewDir*_curCam.getNearClip() + _viewWidth*mousePos.x - _viewHight*mousePos.y;

	// compute direction of picking ray by subtracting intersection point
	// with camera position
	rayDir = rayPos - _curCam.getEyePoint();
}
void MyCam::test(Vec2f mousePos, Vec3f &rayPos, Vec3f &out) {
	// translate mouse coordinates so that the origin lies in the center
	// of the view port
	mousePos -= (getWindowBounds().getSize() / 2);

	// scale mouse coordinates so that half the view port width and height
	// becomes 1
	mousePos /= (getWindowBounds().getSize() / 2);

	// linear combination to compute intersection of picking ray with
	// view port plane
	rayPos = _curCam.getEyePoint() + _viewWidth*mousePos.x - _viewHight*mousePos.y;
	out = _curCam.getEyePoint();
}


CameraPersp& MyCam::getCam() { 
	return _curCam; 
}
void MyCam::setCam(const CameraPersp &camIn ) { 
	_curCam = camIn; 
	update();
}	

void MyCam::update() {
	updatePickingData();
}

Vec2f MyCam::to2D(Vec3f pos) {
	//transform world to clipping coordinates
	Vec3f newPoint = _curCam.getProjectionMatrix() * _curCam.getModelViewMatrix() * pos;
	//Vec2f test = newPoint.xy();
	//Vec2f test2 = getWindowSize();
	//Vec2f out = (test / 2) * test2;
	//return out;

	newPoint.x /= newPoint.z;
    newPoint.y /= newPoint.z;
	newPoint.x = (newPoint.x + 1) * getWindowSize().x / 2;
    newPoint.y = (newPoint.y + 1) * getWindowSize().y / 2;
	return Vec2f(newPoint.x, newPoint.y);
    /*int winX = (int) Math.round((( point3D.getX() + 1 ) / 2.0) * width );
    //we calculate -point3D.getY() because the screen Y axis is
    //oriented top->down 
    int winY = (int) Math.round((( 1 - point3D.getY() ) / 2.0) * height );
    return new Point2D(winX, winY);*/
}

Vec3f MyCam::to3D(Vec2f pos) {
/* 
        double x = 2.0 * winX / clientWidth - 1;
        double y = - 2.0 * winY / clientHeight + 1;
        Matrix4 viewProjectionInverse = inverse(projectionMatrix *
             viewMatrix);

        Point3D point3D = new Point3D(x, y, 0); 
        return viewProjectionInverse.multiply(point3D);
*/
	return Vec3f::zero();
}