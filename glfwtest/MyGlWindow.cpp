

#include "MyGlWindow.h"

#include<iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include<Windows.h>

#include<cmath>
#include<cstdlib>
#include<map>
#include<vector>

#if RVO_OUTPUT_TIME_AND_POSITIONS
#include <iostream>
#endif

#if RVO_SEED_RANDOM_NUMBER_GENERATOR
#include <ctime>
#endif

#if _OPENMP
#include <omp.h>
#endif

#include "RVO.h"
#include<fstream>

const int MAX_SIZE = 16;

#ifndef M_PI
const float M_PI = 3.14159265358979323846f;
#endif

class RoadmapVertex {
public:
	RVO::Vector2 position;
	std::vector<int> neighbors;
	std::vector<float> distToGoal;
};

/* Store the roadmap. */
std::vector<RoadmapVertex> roadmap;

/* Store the goals of the agents. */
std::vector<RVO::Vector2> goals;

using namespace std;

ShaderProgram *shaderProgram;
RVO::RVOSimulator *sim;

static float DEFAULT_VIEW_POINT[3] = { 5, 5, 5 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

/// <summary>
/// 초기 위치 설정.
/// <param name="sim">sim: RVO 시뮬레이터.</param>
/// <param name="scenarioNum">scenarioNum: 시나리오 번호.</param>
/// </summary>
void MyGlWindow:: SetupScenario(RVO::RVOSimulator *sim,int scenarioNum)
{
#if RVO_SEED_RANDOM_NUMBER_GENERATOR
	std::srand(static_cast<unsigned int>(std::time(NULL)));
#endif
	if (scenarioNum == 0)
	{
		/* Specify the global time step of the simulation. */
		sim->setTimeStep(0.1f);
		/* Specify the default parameters for agents that are subsequently added. */
		sim->setAgentDefaults(50.0f, 10, 5.0f, 5.0f, 2.0f, 1.0f);
	}
	else if (scenarioNum == 1)
	{
		sim->setTimeStep(0.01f);
		/* Specify the default parameters for agents that are subsequently added. */
		sim->setAgentDefaults(50.0f, 10, 5.0f, 0.1f, 1.0f, 1.0f);

		std::vector<glm::vec2> obstaclePos;
		obstaclePos.push_back(glm::vec2(-25,-2.0));
		obstaclePos.push_back(glm::vec2(-25, -40));
		obstaclePos.push_back(glm::vec2(25, -40));
		obstaclePos.push_back(glm::vec2(25, -2.0));

		SetObstacle(obstaclePos);

		obstaclePos.clear();
		obstaclePos.push_back(glm::vec2(-25, 40));
		obstaclePos.push_back(glm::vec2(-25, 2));
		obstaclePos.push_back(glm::vec2(25, 2));
		obstaclePos.push_back(glm::vec2(25, 40));

		SetObstacle(obstaclePos);

		for (int i = 0; i < goals.size(); i++)
		{
			goals[i] = RVO::Vector2(27,0);
		}

	}
	else if (scenarioNum == 2)
	{
		sim->setTimeStep(0.2f);
		/* Specify the default parameters for agents that are subsequently added. */
		sim->setAgentDefaults(50.0f, 10, 1.0f,0.5f, 1.0f, 1.0f);

		std::vector<glm::vec2> obstaclePos;
		obstaclePos.push_back(glm::vec2(-25, -10));
		obstaclePos.push_back(glm::vec2(-25, -30));
		obstaclePos.push_back(glm::vec2(-5, -30));
		obstaclePos.push_back(glm::vec2(-5, -10));

		SetObstacle(obstaclePos);

		obstaclePos.clear();
		obstaclePos.push_back(glm::vec2(-25, 30));
		obstaclePos.push_back(glm::vec2(-25, 10));
		obstaclePos.push_back(glm::vec2(-5, 10));
		obstaclePos.push_back(glm::vec2(-5, 30));

		SetObstacle(obstaclePos);

		obstaclePos.clear();
		obstaclePos.push_back(glm::vec2(25, -30));
		obstaclePos.push_back(glm::vec2(25, -10));
		obstaclePos.push_back(glm::vec2(5, -10));
		obstaclePos.push_back(glm::vec2(5, -30));

		SetObstacle(obstaclePos);

		obstaclePos.clear();
		obstaclePos.push_back(glm::vec2(25, 10));
		obstaclePos.push_back(glm::vec2(25, 30));
		obstaclePos.push_back(glm::vec2(5, 30));
		obstaclePos.push_back(glm::vec2(5, 10));

		SetObstacle(obstaclePos);

		for (int i = 0; i < goals.size(); i++)
		{
			goals[i] = RVO::Vector2(27, 0);
		}

	}
}

/// <summary>
/// 초기 위치 설정.
/// <param name="initPos">initPos: 초기 위치.</param>
/// </summary>
void MyGlWindow::SetInitPos(std::vector<glm::vec2> initPos)
{
	for (int i = 0; i < initPos.size(); i++)
	{
		sim->addAgent(RVO::Vector2(initPos[i].x, initPos[i].y));
		mData.push_back(modelData(i, initPos[i]));
	}
	mModelNum = initPos.size();
	mOffset = new glm::mat4[mModelNum];
}

/// <summary>
/// 목표 위치 설정
/// <param name="goalPos">goalPos: 목표 위치.</param>
/// </summary>
void MyGlWindow::SetGoalPos(std::vector<glm::vec2> goalPos)
{
	for (int i = 0; i < goalPos.size(); i++)
	{
		goals.push_back(RVO::Vector2(goalPos[i].x, goalPos[i].y));
		mData[i].addGoalPos(goalPos[i]);
	}
	
}

/// <summary>
/// 방해물 위치 설정
/// <param name="obstaclePos">obstaclePos: 방해물 다각형 위치 vec2 .</param>
/// </summary>
void MyGlWindow::  SetObstacle(std::vector<glm::vec2> obstaclePos)
{
	for(int i=0;i<obstaclePos.size();i++)
		mObstacle4.push_back(RVO::Vector2(obstaclePos[i].x, obstaclePos[i].y));

	mObstRect = new ObstacleRect(mObstacle4);
	mObstRectVec.push_back(mObstRect);

	sim->addObstacle(mObstacle4);
	mObstacleCount++;
	mObstacle4.clear();

	sim->processObstacles();
}

#if RVO_OUTPUT_TIME_AND_POSITIONS
void updateVisualization(RVO::RVOSimulator *sim)
{
	/* Output the current global time. */
	std::cout << sim->getGlobalTime();

	/* Output the current position of all the agents. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		std::cout << " " << sim->getAgentPosition(i);
	}

	std::cout << std::endl;
}
#endif

#if RVO_OUTPUT_TIME_AND_POSITIONS
void updateVisualization(RVO::RVOSimulator *sim)
{
	/* Output the current global time. */
	std::cout << sim->getGlobalTime();

	/* Output the current position of all the agents. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		std::cout << " " << sim->getAgentPosition(i);
	}

	std::cout << std::endl;
}
#endif

/// <summary>
/// 매 프레임마다 객체들의 선호속도를 설정
/// <param name="sim">sim: RVO 시뮬레이터.</param>
/// </summary>
void setPreferredVelocities(RVO::RVOSimulator *sim)
{
	/*
	* Set the preferred velocity to be a vector of unit magnitude (speed) in the
	* direction of the goal.
	*/
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i) {
		RVO::Vector2 goalVector = goals[i] - sim->getAgentPosition(i);

		if (RVO::absSq(goalVector) > 1.0f) {
			goalVector = RVO::normalize(goalVector);
		}

		sim->setAgentPrefVelocity(i, goalVector);

		/*
		* Perturb a little to avoid deadlocks due to perfect symmetry.
		*/
		float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
		float dist = std::rand() * 0.0001f / RAND_MAX;

		sim->setAgentPrefVelocity(i, sim->getAgentPrefVelocity(i) +
			dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
	}
}

/// <summary>
/// 목표지점에 도착했는지 체크하는 함수
/// <param name="sim">sim: RVO 시뮬레이터.</param>
/// <param name="frameNum">frameNum: 현재 프레임 번호.</param>
/// </summary>
bool MyGlWindow:: ReachedGoal(RVO::RVOSimulator *sim,int * frameNum)
{
	int goalCount = 0;
	/* Check if all agents have reached their goals. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		/*if (RVO::absSq(sim->getAgentPosition(i) - goals[i]) > 20.0f * 20.0f) {
			return false;
		}*/
		if (RVO::absSq(sim->getAgentPosition(i) - goals[i]) < 0.1f)
		{
			if (mArrayReachGoalFN[i] == 0)
			{
				mArrayReachGoalFN[i] = *frameNum;
				if (mEndSimulatorFrameNum < *frameNum)
					mEndSimulatorFrameNum = *frameNum;
			}
			else
			{
				goalCount++;
			}
		}
	}
	if (goalCount == sim->getNumAgents())
	{
		return true;
	}
	else
		return false;
}

/// <summary>
/// MyGlWindow 생성자
/// <param name="width">width: 화면 가로크기.</param>
/// <param name="height">height: 화면 세로크기.</param>
/// </summary>
MyGlWindow::MyGlWindow(int w, int h):mFrameNum(0)
//==========================================================================
{
		
	mWidth = w;
	mHeight = h;


	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	mViewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	initialize();
}

/// <summary>
/// MyGlWindow 초기화 함수
/// <param name="scenarioNum">scenarioNum: 시나리오 번호.</param>
/// </summary>
void MyGlWindow::initialize(int scenarioNum)
{
	sim = new RVO::RVOSimulator();
	/* Set up the scenario. */
	SetupScenario(sim, scenarioNum);

	mTriangle = new triangle(0);
	mEllipse = new ellipse(0);
	mGrid = new grid();

	bStart = false;
	bIsLoad = false;
	bFirst = true;
	mFrameNum = 0;
	bIsLoadData = false;
	bSelect = false;
}

/// <summary>
/// View Matrix를 위한 lookAt 함수
/// <param name="pos">pos: 카메라의 위치.</param>
/// <param name="look">look: 어디를 바라보는지.</param>
/// <param name="up">up: up 벡터.</param>
/// <returns>View Matrix</returns>
/// </summary>
glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 R;

	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);


	return R*T;
}

/// <summary>
/// perspective 행렬 계산을 위한 함수
/// <param name="fov">fov: 카메라의 화각.</param>
/// <param name="aspect">aspect: 화면비율.</param>
/// <param name="near">near: 그리는 공간의 시작점까지의 거리.</param>
/// <param name="far">far: 그리는 공간의 끝점까지의 거리.</param>
/// <returns>Perspective Matrix</returns>
/// </summary>
glm::mat4 perspective(float fov, float aspect, float n, float f)
{
	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = -(n + f) / (f - n);
	float B = (2 * (n * f)) / (n - f);

	P[0] = glm::vec4(tanHalfFOV / aspect, 0, 0, 0);
	P[1] = glm::vec4(0, tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, -A, B);
	P[3] = glm::vec4(0, 0, -1.0f, 0.0f);

	return P;

}

std::ifstream& GotoLine(std::ifstream& file, unsigned int num) {
	file.seekg(std::ios::beg);
	for (int i = 0; i < num - 1; i++) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return file;
}

/// <summary>
/// 추가로 설정할 객체 숫자 받아서 저장하는 함수
/// <param name="randAgentNum">randAgentNum: 추가 객체 숫자.</param>
/// </summary>
void MyGlWindow::SetRandAgentNum(int randAgentNum)
{
	this->mRandAgenNum = randAgentNum;
}

/// <summary>
/// 매 프레임마다 MyGlWindow 그리기
/// <param name="m_zoom">m_zoom: 확대한 정도.</param>
/// </summary>
void MyGlWindow::Draw(static float scale,static glm::vec4 RGB, static glm::vec3 LightPos,int m_amount,float m_zoom)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, mWidth, mHeight);

	glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 100.0f/m_zoom), // Camera is at (0,0,5), in World Space
	glm::vec3(0, 0, 0), // and looks at the origin
	glm::vec3(0,-1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 projection = perspective(45.0f, (mWidth /mHeight), 3.0f, 100.0f);

	glm::mat4 trans;

	int colorNum;

	mModel.glPushMatrix();

	glm::mat4 model = mModel.getMatrix();
	
	if (mGrid)
		mGrid->Draw(model, viewMat, projection);

	for (int j = 0; j < mPointGroup.size(); j++)
	{
		mPointGroup[j]->Draw(model, viewMat, projection);
	}

	for (int i = 0; i < mObstRectVec.size(); i++)
	{
		mObstRectVec[i]->Draw(mModel.getMatrix(), viewMat, projection);
	}

	if (bStart)
	{
		if (bIsLoad)
		{
			if (!bIsLoadData)
				loadData(mFrameMax);
			else {
				if (mTriangle)
				{ 
					if (mFrameNum <= mFrameMax&&mFrameNum >= mFrameMin)
					{
						for (int j = 0; j < mModelNum; j++) 
						{
							colorNum = j / (mRandAgenNum + 1);

							mModel.glPushMatrix();
							mModel.glRotate(mData[j].dir[mFrameNum] + 90.0f, 0, 0, 1);
							model = mModel.getMatrix();
							mTriangle->Draw(model, viewMat, projection, mData[j].getPosition(mFrameNum), colorNum);
							//m_ellipse->draw(model, viewMat, projection, mData[j].getPosition(frameNum), colorNum);
							mModel.glPopMatrix();
						}

						if (mEndSimulatorFrameNum >= mFrameNum && !bPause)
							mFrameNum++;
					}	
				}
			}	
		}
		else
		{
			bool isReachGoal = ReachedGoal(sim, &mFrameNum);
			{
#if RVO_OUTPUT_TIME_AND_POSITIONS
				updateVisualization(sim);
#endif
				setPreferredVelocities(sim);
	
				sim->doStep(powfScalar);
				
				if (bFirst == true)
				{
					std::cout << mModelNum << "...";
					//배열 초기화
					for (int i = 0; i < mModelNum; i++)
						mArrayReachGoalFN.push_back(0);
					bFirst = false;
				}
				if (mFrameMin >= mFrameMax)
				{
					std::cout << std::endl << std::endl << "error:: 프레임이 잘못되었습니다. frameMin:"<<mFrameMin<<", frameMax:"<<mFrameMax << std::endl;
					exit(1);
				}

				if (mFrameNum <= mFrameMax&&mFrameNum >= mFrameMin)
				{
					ofstream myFile("data.bin", ios::out | ios::binary | ios::app);
					ofstream myFile2("posdatax.txt", ios::out|ios::app);
					ofstream myFile3("posdatay.txt", ios::out|ios::app);
					if (myFile.good()) {

						myFile.write((char *)&mFrameNum, sizeof(mFrameNum));

						for (unsigned int i = 0; i < sim->getNumAgents(); i++) {
							float x = sim->getAgentPosition(i).x();
							float y = sim->getAgentPosition(i).y();

							myFile.write((char *)&x, sizeof(float));
							myFile.write((char *)&y, sizeof(float));

							myFile2 << (std::to_string(x)+ "\n").c_str();
							myFile3 << (std::to_string(y) + "\n").c_str();

							if (mFrameNum >= mFrameMax)
							{
								std::cout << mFrameNum << "Done!";
								mFrameNum = 0;
							}

							colorNum = i / (mRandAgenNum + 1);

							model = mModel.getMatrix();

							if (mTriangle)
							{
								mTriangle->Draw(model, viewMat, projection, glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)), colorNum);
								//m_ellipse->draw(model, viewMat, projection, glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)), colorNum);
							}
						}
						myFile.close();
						myFile2.close();
						myFile3.close();
					}
				}
				if (!isReachGoal)
				mFrameNum += 1;
			}
		}	
	}
	mModel.glPopMatrix();
}

/// <summary>
/// data.bin 파일에서 객체들의 프레임별 정보 받아오기
/// <param name="frameMax">frameMax: 제일 마지막 프레임.</param>
/// </summary>
void MyGlWindow:: loadData(int frameMax)
{
	mFrameNum = 0;
	ifstream myFile2("data.bin", ios::in | ios::binary);
	if (myFile2.good()) {
		int z;
		for (unsigned int j = 0; j < frameMax; j++)
		{
			myFile2.read((char *)&z, sizeof(int));

			for (unsigned int i = 0; i < mModelNum; i++) {
				float x, y;
				myFile2.read((char *)&x, sizeof(float));
				myFile2.read((char *)&y, sizeof(float));
				//모든 Position값 vector에 저장
				mVecAgentPos.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)));
				//mData의 각각에 position 대입
				mData[i].addPosition(glm::vec2(x,y));
			}	
		}

		for (unsigned int j = 0; j < frameMax; j++)
		{
			for (unsigned int i = 0; i < mModelNum; i++) {
				if (j != frameMax - 1)
					mData[i].calcuAngle(j);
			}
		}

		myFile2.close();
		bIsLoadData = true;
	}
}

/// <summary>
/// 소멸자
/// </summary>
MyGlWindow::~MyGlWindow()
{
	delete mTriangle;
	delete mGrid;
	delete mObstRect;
	delete mOffset;
}
