#include "Test.h"

#include <GL/glew.h>

#include "Debugger.h"
#include "vendor/imgui/imgui.h"

namespace test
{
    void TestMenu::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
        
        if (m_CurrentTest)
        {
            m_CurrentTest->OnUpdate(deltaTime);
        }
    }

    void TestMenu::OnRender()
    {
        Test::OnRender();

        if (m_CurrentTest)
        {
            m_CurrentTest->OnRender();
        }
        else
        {
            GlCall(glClearColor(0.f, 0.f, 0.f, 1.f));
            GlCall(glClear(GL_COLOR_BUFFER_BIT));
        }
    }

    void TestMenu::OnImGuiRender()
    {
        Test::OnImGuiRender();

        if (m_CurrentTest)
        {
            ImGui::Begin(m_TestName.c_str());
            m_CurrentTest->OnImGuiRender();
            if (m_CurrentTest && ImGui::Button("<-"))
            {
                m_CurrentTest.reset();
            }
            ImGui::End();
        }
        else
        {
            for (auto&& TestPair : m_TestDict)
            {
                m_TestName = TestPair.first;
                if (ImGui::Button(TestPair.first.c_str()))
                    m_CurrentTest = TestPair.second();
            }
        }
    }
}
