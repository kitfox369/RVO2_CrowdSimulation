
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imguifilesystem.h"

#include "gl3w.h"
#include<GLFW\glfw3.h>
#include<iostream>
#include<string>
#include<vector>
#include<sstream>

#include <windows.h>

#include "MyGlWindow.h"

MyGlWindow * win;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;
int frameMin,frameMax;
bool setFirst = true;
int order = 0;

static void ShowExampleAppMainMenuBar();
static void ShowExampleMenuFile();
static void ShowExampleAppLayout(bool* p_open);
void agentPosAdd(double tempX, double tempY);


static bool show_app_layout = false;

int agentAmount = 0;						//객체의 수
int randAgentNum;
int randRange;

int width = 800;
int height = 800;

float m_zoom = 50.0f;

std::vector<glm::vec2> selectPos;

static void ShowExampleAppFixedOverlay(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(800, 20));
	if (!ImGui::Begin("Example: Fixed Overlay", p_open, ImVec2(200, 800), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{

		ImGui::End();
		return;
	}

	ImGui::Text("Mouse Position: \n (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	ImGui::Separator();
	ImGui::Text(u8"Application Average: \n %.3f ms/frame \n(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();

	if (order == 0)
	{
		ImGui::TextColored(ImVec4(1, 1, 0.3, 0.7), u8"자동으로 추가 생성될 인원과\n범위를 정하세요");
		ImGui::SliderInt(u8"인원", &randAgentNum, 1, 100);
		ImGui::SliderInt(u8"범위", &randRange, 1, 100);
		if (ImGui::Button(u8"선택", ImVec2(100, 50)))
		{
			win->SetRandAgentNum(randAgentNum);
			order++;
		}
	}
	else if (order==1)
	{
		ImGui::TextColored(ImVec4(1,1,0.3,0.7),u8"초기 위치를 클릭해주세요");
		if (ImGui::Button(u8"초기위치 선택", ImVec2(100, 50)))
		{
			if (selectPos.size() > 0)
			{
				win->SetInitPos(selectPos);
				agentAmount = selectPos.size();
				selectPos.clear();
				win->mPointGroup.clear();
				order++;
			}
		}
		if (ImGui::Button(u8"삭제", ImVec2(100, 50)))
		{
			selectPos.erase(selectPos.begin() + (selectPos.size() - 1));
			win->mPointGroup.erase(win->mPointGroup.begin() + (win->mPointGroup.size() - 1));
		}
	}
	else if (order==2)
	{
		ImGui::TextColored(ImVec4(1, 1, 0.3, 0.7), u8"목표 위치를 클릭해주세요");
		ImGui::TextColored(ImVec4(1, 1, 0.3, 0.7), u8"미선택시 (0,0)으로 객체 수만큼 초기화");
		if (ImGui::Button(u8"목표위치 선택", ImVec2(100, 50)))
		{
			if (selectPos.size() > 0)
			{
				win->SetGoalPos(selectPos);
				selectPos.clear();
				win->mPointGroup.clear();
				order++;
			}
			else
			{
				for (int i = 0; i < agentAmount; i++)
				{
					selectPos.push_back(glm::vec2(0,0));
				}
				win->SetGoalPos(selectPos);
				selectPos.clear();
				win->mPointGroup.clear();
				order++;
			}
		}
		if (ImGui::Button(u8"삭제", ImVec2(100, 50)))
		{
			selectPos.erase(selectPos.begin() + (selectPos.size() - 1));
			win->mPointGroup.erase(win->mPointGroup.begin() + (win->mPointGroup.size() - 1));
		}
	}
	else if (order==3)
	{
		ImGui::TextColored(ImVec4(1, 1, 0.3, 0.7), u8"방해물 위치를 클릭해주세요\n(반시계방향으로 꼭지점 클릭)");
		if (ImGui::Button(u8"선택완료", ImVec2(100, 50)))
		{
			if (setFirst)
			{
				win->SetObstacle(selectPos);
				selectPos.clear();
				win->bSelect = true;
			}
		}
		if (ImGui::Button(u8"설치완료", ImVec2(100, 50)))
		{
			setFirst = false;
			order++;
		}
		if (ImGui::Button(u8"삭제", ImVec2(100, 50)))
		{
			selectPos.erase(selectPos.begin() + (selectPos.size() - 1));
			win->mPointGroup.erase(win->mPointGroup.begin() + (win->mPointGroup.size() - 1));
		}
	}
	else if (order == 4)
	{
		//if (ImGui::Button(u8"새로만들기", ImVec2(100, 50)))
		if (ImGui::Button(u8"New", ImVec2(100, 50)))
		{
			std::remove("data.bin");
			for (int i = 0; i < win->mArrayReachGoalFN.size(); i++)			//목표 가까이에 도착한 프레임 저장 배열 리셋
				win->mArrayReachGoalFN[i] = 0;
			win->bStart = true;
			win->mFrameNum = 0;
			win->bIsLoad = false;
		}

		//if (ImGui::Button(u8"불러오기", ImVec2(100, 50)))
		if (ImGui::Button(u8"Load", ImVec2(100, 50)))
		{
			win->mFrameNum = 0;
			win->bIsLoad = true;
			win->bStart = true;
		}
		//if (ImGui::Button(u8"시작", ImVec2(100, 50)))
		if (ImGui::Button(u8"Start", ImVec2(100, 50)))
		{
			win->bPause = false;
		}
		//if (ImGui::Button(u8"일시정지", ImVec2(100, 50)))
		if (ImGui::Button(u8"Pause", ImVec2(100, 50)))
		{
			win->bPause = true;
		}
		ImGui::Separator();

		ImGui::SliderFloat(u8"powf", &win->powfScalar, 0, 5);
		//std::cout << "Source powfScalar:" << powfScalar << std::endl;
		ImGui::Separator();

		ImGui::SliderInt(u8"start\nframe",&win->mFrameMin, 0, 10000);
		ImGui::SliderInt(u8"end\nframe", &win->mFrameMax, 0, 10000);
		ImGui::SliderInt(u8"current\nframe", &win->mFrameNum, 0, win->mEndSimulatorFrameNum);

		if (selectPos.size() > 0)
		{
			ImGui::Text(u8"Clicked Vertex");
			for (int i = 0; i < selectPos.size(); i++)
			{
				ImGui::Text("(%.1f,%.1f)", selectPos[i].x, selectPos[i].y);
			}
		}
	}

	ImGui::Separator();
	ImGui::Text(u8"줌인: X%.3f ", m_zoom);

	ImGui::End();
}

static void ShowExampleAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Senario"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ModeNum"))
		{
			if (ImGui::MenuItem("Camera ZoomIn x2.0", u8"카메라 줌인")) { m_zoom *= 2.0f; }
			if (ImGui::MenuItem("Camera ZoomOut x0.5", u8"카메라 줌아웃")) { m_zoom /= 2.0f; }
			if (ImGui::MenuItem("Origin Camera", u8"카메라 원상태")) { m_zoom = 1.0f; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void ShowExampleAppLayout(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Example: Layout", p_open, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				const bool browseButtonPressed = ImGui::Button("...");                          // we need a trigger boolean variable
				static ImGuiFs::Dialog dlg;                                                     // one per dialog (and must be static)
				const char* chosenPath = dlg.chooseFileDialog(browseButtonPressed);             // see other dialog types and the full list of arguments for advanced usage
				if (strlen(chosenPath)>0) {
					// A path (chosenPath) has been chosen RIGHT NOW. However we can retrieve it later more comfortably using: dlg.getChosenPath()
					//win->changeModel("");
				}
				if (strlen(dlg.getChosenPath())>0) {
					ImGui::Text("Chosen file: \"%s\"", dlg.getChosenPath());
				}

				// If you want to copy the (valid) returned path somewhere, you can use something like:
				static char myPath[ImGuiFs::MAX_PATH_BYTES];
				if (strlen(dlg.getChosenPath())>0) {
					strcpy(myPath, dlg.getChosenPath());
				}

				if (ImGui::MenuItem("Close")) *p_open = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// left
		static int selected = 0;
		ImGui::BeginChild("left pane", ImVec2(300, 0), true);
		for (int i = 0; i < 100; i++)
		{
			char label[128];
			sprintf(label, "MyObject %d", i);
			if (ImGui::Selectable(label, selected == i))
				selected = i;
		}
		ImGui::EndChild();
		ImGui::SameLine();

		// right
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Text("MyObject: %d", selected);
		ImGui::Separator();
		ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
		ImGui::EndChild();
		ImGui::BeginChild("buttons");
		if (ImGui::Button("Select")) {}
		ImGui::SameLine();
		if (ImGui::Button("Close")) { *p_open = false; }
		ImGui::EndChild();
		ImGui::EndGroup();
	}
	ImGui::End();
}
// Demonstrate creating a window which gets auto-resized according to its content.
static void ShowExampleAppAutoResize(bool* p_open)
{
	if (!ImGui::Begin("Example: Auto-resizing window", p_open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}

	static int lines = 10;
	ImGui::Text("Window will resize every-frame to the size of its content.\nNote that you probably don't want to query the window size to\noutput your content because that would create a feedback loop.");
	ImGui::SliderInt("Number of lines", &lines, 1, 20);
	for (int i = 0; i < lines; i++)
		ImGui::Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
	ImGui::End();
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	win->SetSize(width, height);
	win->SetAspect(width / (float)height);
	
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

//마우스위치를 OpenGl 위치로 변환( 오른쪽 위가 0,0)
void transToModelPosition(float x, float y)
{
	float halfW = (width/2.0f)*m_zoom;
	float halfH = (height/2.0f)*m_zoom;
	float tempX, tempY;

	tempX = halfW - x;
	tempY =y-halfH;

	//2500 / 4.0f=640; 50*12
	if (order == 1 || order == 2)
		agentPosAdd(tempX /(m_zoom*m_zoom/3.7f), tempY / (m_zoom*m_zoom / 3.7f));
	else if (order == 3)
	{
		glm::vec2 tempVec2 = glm::vec2(tempX/ (m_zoom*m_zoom / 3.7f), tempY/ (m_zoom*m_zoom / 3.7f));
		selectPos.push_back(tempVec2);
		win->mPointGroup.push_back(new ObstaclePoint(tempVec2));
	}
}

void agentPosAdd(double tempX, double tempY)
{
	//selectPos는 선택된 position vector, pointGroup은 그릴 vector
	glm::vec2 tempVec2 = glm::vec2(tempX, tempY);
	selectPos.push_back(tempVec2);
	win->mPointGroup.push_back(new ObstaclePoint(tempVec2));

	glm::vec2 additoryVec2;

	for (int i = 0; i < randAgentNum; i++)
	{
		int randTempX = rand()%randRange;
		int randTempY = rand() % randRange;
		int minTemp[2];
		for(int j=0;j<2;j++)
		{
			if(rand() % 2==0)
				minTemp [j]= -1;
			else
				minTemp[j] = 1;
		}

		additoryVec2 = glm::vec2(tempX+(randTempX*minTemp[0]), tempY+(randTempY*minTemp[1]));

		selectPos.push_back(additoryVec2);
		win->mPointGroup.push_back(new ObstaclePoint(additoryVec2));
	}

}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//버튼 눌릴때
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
		if (ImGui::GetIO().MousePos.x <= width&& ImGui::GetIO().MousePos.y <= height)
		{
			transToModelPosition(ImGui::GetIO().MousePos.x*m_zoom, ImGui::GetIO().MousePos.y*m_zoom);
		}
	}
	
	//마우스 왼쪽 버튼 클릭
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}
	//마우스 오른쪽 버튼 클릭
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}
	//마우스 휠 클릭
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}	
}

void mouseDragging(double width, double height)		//카메라 회전
{
	
	if (ImGui::GetIO().WantCaptureKeyboard == false) {
		if (lbutton_down) {
			float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
			float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
			win->mViewer->rotate(fractionChangeX, fractionChangeY);
		}
		else if (mbutton_down) {
			float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
			float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
			win->mViewer->zoom(fractionChangeY);
		}
		else if (rbutton_down) {
			float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
			float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
			win->mViewer->translate(-fractionChangeX, -fractionChangeY, 1);
		}
	}

	m_lastMouseX = cx;
	m_lastMouseY = cy;

}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);		//OpenGl 4.0을 쓰겠다
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);		//3.0
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//CORE_PROFILE : 최신의 mordern한 기능만 가져다가 쓰는것		COMFORT_PROFILE : 예전 버전도 있음


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width+200, height, "OpenGL FrameWork", NULL, NULL);	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}

	if (!gl3wIsSupported(4, 3)) {
		fprintf(stderr, "OpenGL 4.0 not supported\n");
		return -1;
	}

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	win = new MyGlWindow(width, height);

	
	bool show_test_window = true;
	bool show_another_window = true;
	bool show_menubar = false;
	bool show_app_main_menu_bar = true;
	
	ImVec4 clear_color = ImColor(144, 144, 144);
	
	double previousTime = glfwGetTime();
	int frameCount = 0;
	double lastTime;

	ImGuiIO& io = ImGui::GetIO();
	
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	//fltk와 다르게 한글이 지원된다. 
	glfwSetWindowTitle(window, "RVO2_CrowdSimulation");

	io.Fonts->AddFontFromFileTTF("BMJUA_ttf.ttf", 17.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplGlfwGL3_NewFrame();

		static float f = 0.5f;

		static glm::vec4 RGB = glm::vec4(1, 1, 1, 1);
		static glm::vec3 LightPos = glm::vec3(50, 50, 50);

		static bool isAlpha = false;

		static bool show_app_main_menu_bar = true;

		if (show_app_main_menu_bar) ShowExampleAppMainMenuBar();
		if (show_app_layout) ShowExampleAppLayout(&show_app_layout);

		static bool no_titlebar = false;

		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;

		if (show_another_window)
		{
			ShowExampleAppFixedOverlay(&show_another_window);
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		win->Draw(f,RGB,LightPos, agentAmount, m_zoom);

		ImGui::Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		//glfwWaitEvents();
		mouseDragging(display_w, display_h);
		
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

static void ShowExampleMenuFile()
{
	if (ImGui::MenuItem("Basic"))
	{
		win->initialize();
	}
	if (ImGui::MenuItem("Line"))
	{
		win->initialize(2);
	}
	
}