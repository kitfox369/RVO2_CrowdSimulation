
#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include<iostream>
#include<vector>
#include<glm\glm.hpp>
#include<math.h>

class modelData
{
private:
	int mId;									//몇번째 사람인가
	glm::vec2 mInitPos;				//맨처음 Position	
	glm::vec2 mGoalPos;						//goal Position

	/// initPos : 처음 시작하는 위치, goalPos : 마지막 목표 위치
	void initialize(int id,glm::vec2 initPos)
	{
		this->mInitPos = initPos;
		this->mId = id;
	}

	void modelSetGoalPos(glm::vec2 goalPos)
	{
		this->mGoalPos = goalPos;
	}

public:
	std::vector<glm::vec2> position;		//position
	std::vector<float> dir;
	// int reachGoalFN = 0;
	

	modelData() {};
	modelData(int id,glm::vec2& initPos)
	{
		initialize(id,initPos);
	};

	void addGoalPos(glm::vec2& goalPos)
	{
		modelSetGoalPos(goalPos);
	}

	//float calcuAngle(RVO::Vector2 dir)
	//{
	//	//std::cout << atan2(dir.y() - simPos.y(), dir.x() - simPos.x())*(180 / 3.1415f) << " ";
	//	return atan2(dir.y() ,dir.x())*(180 / 3.1415f);	
	//}

	void calcuAngle(int frameNum)
	{
		//std::cout << atan2(dir.y() - simPos.y(), dir.x() - simPos.x())*(180 / 3.1415f) << " ";
		//dir.push_back(atan2(0- position[frameNum+1].y, 0- position[frameNum+1].x)*(180 / 3.1415f));
		glm::vec2 next = glm::vec2(position[frameNum + 1].x, position[frameNum + 1].y);
		glm::vec2 current = glm::vec2(position[frameNum].x,position[frameNum].y);
		glm::vec2 d = current-next;
		d = glm::normalize(d);
		dir.push_back(atan2f(d.y, d.x) * (180.0f / 3.1415f));
		//std::cout << " "<< atan2f(d.y, d.x)*(180.0f / 3.1415f);
	}
	
	///position : 프레임 당 위치 값을 matrix로 변환한것
	void addPosition( glm::vec2& position)
	{
		this->position.push_back(position);
	}

	void addDirection(glm::vec2 &pos)
	{
		//glm::vec2 temp = glm::vec2(velo.x(), velo.y());
		//glm::vec2 temp = 
		//this->dir.push_back(temp);
	}

	glm::mat4 getPosition(int index)
	{
		return glm::translate(glm::mat4(1.0f),glm::vec3(position[index].x, position[index].y,0.0f));
	}

};

#endif // !_MODELDATA_H_
