#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace test
{
    class Test
    {
    public:
        Test() = default;
        virtual ~Test() = default;

        virtual void OnUpdate(float deltaTime = 0.f) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };

    class TestMenu: public Test
    {
    public:
        TestMenu() = default;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

        template <typename T>
        void Register(const std::string& TestName)
        {
            m_TestDict.push_back(std::make_pair(TestName, []() { return std::make_unique<T>(T()); }));
        }

    private:
        std::unique_ptr<Test> m_CurrentTest;
        std::string m_TestName;
        std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> m_TestDict;
    };
}
