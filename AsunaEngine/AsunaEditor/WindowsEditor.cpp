#include "WindowsEditor.h"
#include "AsunaEngine/Graphics/Renderer.h"
#include "AsunaEngine/GUI/GUI.h"
#include <imgui.h>


using namespace asuna;

void asuna::WindowsEditor::Render()
{
	Renderer::Current->Render();
	GUI::Begin();
	RenderEditor();
	GUI::End();
	Renderer::Current->Present();
}

void asuna::WindowsEditor::RenderEditor()
{
	static bool show_another_window;
	ImGui::ShowDemoWindow();

	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close Me"))
		show_another_window = false;
	ImGui::End();
}
