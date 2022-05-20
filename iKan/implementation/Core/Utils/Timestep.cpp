//
//  Timestep.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Timestep.hpp"

using namespace iKan;

// Implementation for timestep

Timestep::Timestep(float time) : m_Time(time) {}
Timestep::operator float() const { return m_Time; }

float Timestep::GetSeconds() const { return m_Time; }
float Timestep::GetMilliSeconds() const { return m_Time * 1000.0f; }
float Timestep::GetMicroSeconds() const { return m_Time * 1000000.0f; }
