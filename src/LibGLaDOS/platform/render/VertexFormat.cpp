#include "VertexFormat.h"
#include "utils/Utility.h"
#include "CommonTypes.h"

namespace GLaDOS {
    Logger* VertexFormat::logger = LoggerRegistry::getInstance().makeAndGetLogger("VertexFormat");
    VertexFormat::VertexFormat(VertexSemantic _semantic, VertexAttributeType _attributeType) : mSemantic{_semantic}, mType{_attributeType} {
    }

    VertexSemantic VertexFormat::semantic() const {
        return mSemantic;
    }

    VertexAttributeType VertexFormat::type() const {
        return mType;
    }

    std::size_t VertexFormat::size() const {
        return CommonTypes::vertexArrtibTypeToSize(mType);
    }

    std::size_t VertexFormat::sizeAlign4() const {
        return align4(size());
    }

    //////////////////////////////////////////////////////////////
    //// VertexFormatHolder definition
    //////////////////////////////////////////////////////////////

    VertexFormatHolder::VertexFormatHolder(Vector<VertexFormat*>& vertexFormats) : mVertexFormats{vertexFormats} {
    }

    VertexFormatHolder::~VertexFormatHolder() {
        deallocIterable(mVertexFormats);
    }

    VertexFormatHolder::iterator VertexFormatHolder::begin() {
        return mVertexFormats.begin();
    }

    VertexFormatHolder::const_iterator VertexFormatHolder::begin() const {
        return mVertexFormats.cbegin();
    }

    VertexFormatHolder::iterator VertexFormatHolder::end() {
        return mVertexFormats.end();
    }

    VertexFormatHolder::const_iterator VertexFormatHolder::end() const {
        return mVertexFormats.cend();
    }

    Vector<VertexFormat*> VertexFormatHolder::getVertexFormats() const {
        return mVertexFormats;
    }

    //////////////////////////////////////////////////////////////
    //// VertexFormatDescriptor definition
    //////////////////////////////////////////////////////////////

    VertexFormatDescriptor VertexFormatDescriptor::position() {
        mUsePosition = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::normal() {
        mUseNormal = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::tangent() {
        mUseTangent = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::biTangent() {
        mUseBiTangent = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::boneWeight() {
        mUseBoneWeight = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::boneIndex() {
        mUseBoneIndex = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord(uint32_t index) {
        switch (index) {
            case 0:
                mUseTexCoord0 = true;
                break;
            case 1:
                mUseTexCoord1 = true;
                break;
            case 2:
                mUseTexCoord2 = true;
                break;
            case 3:
                mUseTexCoord3 = true;
                break;
            case 4:
                mUseTexCoord4 = true;
                break;
            case 5:
                mUseTexCoord5 = true;
                break;
            case 6:
                mUseTexCoord6 = true;
                break;
            case 7:
                mUseTexCoord7 = true;
                break;
        }
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord0() {
        mUseTexCoord0 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord1() {
        mUseTexCoord1 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord2() {
        mUseTexCoord2 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord3() {
        mUseTexCoord3 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord4() {
        mUseTexCoord4 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord5() {
        mUseTexCoord5 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord6() {
        mUseTexCoord6 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::texCoord7() {
        mUseTexCoord7 = true;
        return *this;
    }

    VertexFormatDescriptor VertexFormatDescriptor::color() {
        mUseColor = true;
        return *this;
    }

    VertexFormatHolder* VertexFormatDescriptor::makeVertexFormatHolder() const {
        Vector<VertexFormat*> vertexFormats;

        if (mUsePosition) {
            mPositionOffset = 0;  // position offset always zero
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Position, VertexAttributeType::Float3)));
        }

        if (mUseNormal) {
            mNormalOffset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Normal, VertexAttributeType::Float3)));
        }

        if (mUseTangent) {
            mTangentOffset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Tangent, VertexAttributeType::Float3)));
        }

        if (mUseBiTangent) {
            mBiTangentOffset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::BiTangent, VertexAttributeType::Float3)));
        }

        if (mUseBoneWeight) {
            mBoneWeightOffset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::BoneWeight, VertexAttributeType::Float4))); // maximum 4 bone weight
        }

        if (mUseBoneIndex) {
            mBoneIndexOffset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::BoneIndex, VertexAttributeType::Int4))); // maximum 4 bone index
        }

        if (mUseTexCoord0) {
            mTexCoord0Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord0, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord1) {
            mTexCoord1Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord1, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord2) {
            mTexCoord2Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord2, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord3) {
            mTexCoord3Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord3, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord4) {
            mTexCoord4Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord4, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord5) {
            mTexCoord5Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord5, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord6) {
            mTexCoord6Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord6, VertexAttributeType::Float2)));
        }

        if (mUseTexCoord7) {
            mTexCoord7Offset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::TexCoord7, VertexAttributeType::Float2)));
        }

        if (mUseColor) {
            mColorOffset = VertexFormatDescriptor::sumOfPreviousOffset(vertexFormats);
            vertexFormats.emplace_back(NEW_T(VertexFormat(VertexSemantic::Color, VertexAttributeType::Float4)));
        }

        return NEW_T(VertexFormatHolder{vertexFormats});
    }

    std::size_t VertexFormatDescriptor::sumOfPreviousOffset(const Vector<VertexFormat*>& vertexFormats) {
        std::size_t sum = 0;
        for (const auto& vertexFormat : vertexFormats) {
            sum += vertexFormat->size();
        }
        return sum;
    }
}  // namespace GLaDOS