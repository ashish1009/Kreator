//
//  Entrypoint.h
//  iKan
//
//  Created by iKan on 19/05/22.
//

#pragma once

extern std::unique_ptr<iKan::
Application> iKan::CreateApplication();

/// Core Entiry point defiend here. If this file is included in the Client project then there is no need to add
/// other entry point, otherwise an entiry point must be defined in client
int main() {
    std::cout << "Hello Project \n";

    // Initialize the Logger
    iKan::Logger::Init();
    IK_INFO("Initialized iKan Engine Logger ... ");
    
    // Create application
    auto app = iKan::CreateApplication();
    
    return 0;
}

