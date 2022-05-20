//
//  iKanCommonPreprocessors.h
//  iKan
//
//  Created by iKan on 20/05/22.
//

#pragma oncee

// Include any all Common PAI, Preprocessor, enum or structures that need to be defined in both COre and Client

#define BIT(x) (1 << x) // Shift bit 1 at position x
#define ToString(x) #x  // Convert X to string

/// Supported OS by iKan Engine
enum class OS : uint8_t {
    None = 0,
    Mac = 1
    /* Add More supported OS */
};
