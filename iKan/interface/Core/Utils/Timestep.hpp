//
//  TimeStep.h
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
    /// A Wrapper class to store the time difference between two frame.
    class Timestep {
    public:
        Timestep(float time = 0.0f);
        ~Timestep() = default;
        
        operator float() const;
        
        float GetSeconds() const;
        float GetMilliSeconds() const;
        float GetMicroSeconds() const;

    private:
        float m_Time = 0.0f;
    };
    
}
