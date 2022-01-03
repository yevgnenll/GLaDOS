#include "MetalShader.h"
#include "platform/apple/CocoaPlatform.h"

namespace GLaDOS {
    MetalShader::MetalShader(const std::string& sourceCode) : Shader{sourceCode} {
    }

    MetalShader::~MetalShader() {
        if (isCompiled()) {
            [mFunction release];
        }
    }

    bool MetalShader::createShader() {
        id<MTLDevice> device = MetalRenderer::getInstance().getDevice();
        if (device == nil) {
            LOG_ERROR(logger, "Invalid Metal device state");
            return false;
        }

        NSString* shaderSource = CocoaPlatform::toString(mShaderSourceCode);
        MTLCompileOptions* compileOptions = [MTLCompileOptions new];
        compileOptions.languageVersion = MTLLanguageVersion1_1;
        NSError* compileError;
        id<MTLLibrary> library = [device newLibraryWithSource:shaderSource options:compileOptions error:&compileError];
        [compileOptions release];

        if (compileError != nil) {
            NSString* errorMessage = [NSString stringWithFormat:@"%@", compileError];
            LOG_ERROR(logger, [errorMessage UTF8String]);
            [library release];
            return false;
        }

        mFunction = [library newFunctionWithName:@"main0"];
        [library release];
        if (mFunction == nil) {
            LOG_ERROR(logger, "Invalid function name main0");
            return false;
        }

        mIsCompiled = true;

        return true;
    }

    id<MTLFunction> MetalShader::getFunction() {
        return mFunction;
    }
}