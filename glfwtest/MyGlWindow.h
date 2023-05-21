#pragma warning(push)
#pragma warning(disable:4311)		// convert void* to long
#pragma warning(disable:4312)		// convert long to void*


#include <windows.h>
#include <iostream>

#include "gl3w.h"
#include <GL/gl.h>
#include <string>

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "Loader.h"
#include "Sphere.h"
#include"triangle.h"
#include"ellipse.h"
#include"grid.h"
#include"obstPoint.h"

#include "Viewer.h"
#include "ModelView.h"

#include"modelData.h"

#include"ObstacleRect.h"

#pragma warning(pop)


class MyGlWindow  {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow();
	void Draw(static float scale, static glm::vec4 RGB, static glm::vec3 LightPos,
		int m_amount,float m_zoom);
	void SetRandAgentNum(int randAgentNum);
	void SetSize(int w, int h) { mWidth = w; mHeight = h; }
	void SetAspect(float r) { mViewer->setAspectRatio(r); }
	void SetReachGoalFN();

	void SetupScenario(RVO::RVOSimulator *sim, int scenarioNum);
	void SetObstacle(std::vector<glm::vec2> obstaclePos);
	void SetInitPos(std::vector<glm::vec2> initPos);
	void SetGoalPos(std::vector<glm::vec2> goalPos);
	void loadData(int frameMax);

	std::vector<ObstaclePoint*> mPointGroup;

	bool bStart,bIsLoad,bSelect, bPause;
	 int mFrameNum=0,mFrameMax=0,mFrameMin=0, mEndSimulatorFrameNum = 0;

	 std::vector<int> mArrayReachGoalFN;
	 bool ReachedGoal(RVO::RVOSimulator *sim, int * frameNum);

	glm::mat4 * mOffset ;

	Viewer* mViewer;

	float powfScalar = 2.5f;

	void initialize(int scenarioNum = 0);

private:	

	bool bFirst;

	int mWidth;
	int mHeight;

	int mModelNum;
	int mRandAgenNum;

	int mObstacleCount=0;

	Sphere *mSphere;
	triangle *mTriangle;
	ellipse *mEllipse;

	grid *mGrid;

	Model mModel;

	std::vector<modelData> mData;

	ObstacleRect* mObstRect;

	std::vector<ObstacleRect*> mObstRectVec;
	std::vector<RVO::Vector2>  mObstacle4;
	std::vector<glm::mat4> mVecAgentPos;
	
	bool bIsLoadData;
};
