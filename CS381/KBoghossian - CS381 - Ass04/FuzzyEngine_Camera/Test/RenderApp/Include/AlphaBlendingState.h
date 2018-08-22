#ifndef ALPHABLENDINGSTATE_H
#define ALPHABLENDINGSTATE_H

enum BlendFactor
{
	BlendFactorZero,
	BlendFactorOne,
	BlendFactorSrcColor,
	BlendFactorInvSrcColor,
	BlendFactorDstColor,
	BlendFactorInvDstColor,
	BlendFactorSrcAlpha,
	BlendFactorInvSrcAlpha,
	BlendFactorDstAlpha,
	BlendFactorInvDstAlpha,
	BlendFactorSrcAlphaSaturate
};

class AlphaBlendingState
{
public:

    AlphaBlendingState(BlendFactor srcBlendFactor = BlendFactorSrcAlpha, BlendFactor dstBlendFactor = BlendFactorInvSrcAlpha, bool status = false)
	{
        SrcBlendFactor = srcBlendFactor;
		DstBlendFactor = dstBlendFactor;
		Status = status;
	}

	BlendFactor SrcBlendFactor;
	BlendFactor DstBlendFactor;
	bool Status;
};


#endif