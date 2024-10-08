#include <iostream>
#include "agent.hpp"
#include "mmanager.hpp"
#include "stochastic_decorator.hpp"
#include "simple_decorator.hpp"

#include <vector>
#include <string>
#include "tinyxml2.h"
#include "special_agent.hpp"

class InitManager
{
public:    
    InitManager(){}

    // xml 파일 경로 받아서 초기화
    InitManager(std::string path){
        tinyxml2::XMLDocument doc; // xml 문서 객체 생성
        doc.LoadFile( path.c_str() ); // 정해둔 경로에 xml 파일 로드

        // xml에서 AgentList 찾기
        tinyxml2::XMLElement* agListElem = doc.FirstChildElement( "scenario" )->FirstChildElement( "AgentList" );

        double x, y, heading, speed, drange; // agent의 속성 변수 선언

        // agent 요소를 반복해서 속성 값 읽기
        for (tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
            agElem != NULL; agElem = agElem->NextSiblingElement())
            {
                // xml에서 속성 값 읽기
                agElem->QueryDoubleAttribute("x", &x);
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);

                // 읽은 속성으로 CAgent 객체 생성
                CAgent* ag = new CSpecialAgent(x,  y, heading, speed, drange);
                m_agent_list.push_back(ag); // 생성된 agent를 list에 추가
            }    
    }
private:
    std::vector<CAgent*> m_agent_list;  // CAgent 포인터를 저장하는 vector

public:
    // agent_list를 반환
    std::vector<CAgent*>& get_agent_list() { return m_agent_list; }      
};

int main(int argc, char** argv)
{// Clinet Code
 // 클래스 타입을 몰라도 동작하는 이유(다형성): CAgent가 부모 클래스, CSpecialAgent가 자식 클래스이므로
 // 자식 클래스는 부모 클래스의 모든 속성과 메서드를 상속받는다.
 // CAgnet* 타입의 포인터를 사용해서 CSpecialAgent 객체를 생성했으므로 CAgent 클래스의 타입만 안다면
 // 어떤 자식 클래스의 객체를 사용해도 문제가 없다.

    InitManager init_manager("test.xml");

    /* Simulation Engine */
    CManeuverManager mmanager;

    // 초기화된 agent_list를 등록
    for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter)
    {
        mmanager.register_publisher(*iter); // 각 agent를 mmanager에 등록
    }    
    // XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
    // XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement( "attributeApproach" );
	// attributeApproachElement->QueryIntAttribute( "v", &v0 );

    // CAgnet* p_ag = ???.create_agent(10,  0, 0, 1, 10);
    // m_agent_list.push_back(p_ag]);
    // CAgent* p_agent1 = new CAgent(10,  0, 0, 1, 10);
    // p_agent1 = new StochasticDecorator(p_agent1);
    // p_agent1 = new SimpleDecorator(p_agent1);
    
    // mmanager.register_publisher(p_agent1);

    // CAgent* p_agent2 = new CAgent(10,  30, 180, 1, 10);
    // mmanager.register_publisher(p_agent2);
    
    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;

    for(sim_time = 0; sim_time < 30; sim_time += time_step)
    {   
        //p_agent1->maneuver(time_step);
        //p_agent2->maneuver(time_step);

        //p_agent1->detect(p_agent2);
        //p_agent2->detect(p_agent1);

        mmanager.svc(time_step);

        // std::cout << "----" << std::endl;
        // std::cout << "Time: " << sim_time << ", " <<*p_agent1 << std::endl;
        // std::cout << "Time: " << sim_time << ", " <<*p_agent2 << std::endl;

        std::cout << "----" << std::endl;
        for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter)
        {
            std::cout << "Time: " << sim_time << ", " <<*(*iter) << std::endl;
        }
    }
    return 0;
}
