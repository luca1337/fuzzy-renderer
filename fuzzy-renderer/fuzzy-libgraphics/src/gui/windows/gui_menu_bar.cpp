#include <imgui.h>
#include <gui/windows/gui_menu_bar.h>

namespace libgraphics::gui
{
	auto GUIMenuBar::Render() -> void
	{
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Apri", nullptr))
                {
                    // Gestisci l'evento di apertura del file
                }

                if (ImGui::MenuItem("Salva", nullptr))
                {
                    // Gestisci l'evento di salvataggio del file
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Opzioni"))
            {
                if (ImGui::MenuItem("Opzione 1", nullptr))
                {
                    // Gestisci l'evento dell'opzione 1
                }

                if (ImGui::MenuItem("Opzione 2", nullptr))
                {
                    // Gestisci l'evento dell'opzione 2
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
	}
}
