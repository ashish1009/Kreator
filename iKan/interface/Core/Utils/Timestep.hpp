//
//  TimeStep.h
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma once

namespace iKan {
    
    /// A Wrapper class to store the time difference between two frame.
    /// NOTE: this class only store the time stamp, it is not any counter to update the tim eon its own
    /// Time need to be updated each frame
    /// - Window and Application class sotring the time step of each frame, passing to all Layer via Update(Timestep)
    class Timestep {
    public:
        /// Times Step Constructor
        /// @param time Initial time to be set
        Timestep(TIME time = 0.0f);
        ~Timestep() = default;
        
        /// Return the Time in type TIME
        operator TIME() const;
        
        /// Get the time step (Time took to render 1 Frame) in Seconds
        TIME GetSeconds() const;
        /// Get the time step (Time took to render 1 Frame) in mili Seconds
        TIME GetMilliSeconds() const;
        /// Get the time step (Time took to render 1 Frame) in micro Seconds
        TIME GetMicroSeconds() const;

    private:
        TIME m_Time = 0.0f;
    };
    
}
