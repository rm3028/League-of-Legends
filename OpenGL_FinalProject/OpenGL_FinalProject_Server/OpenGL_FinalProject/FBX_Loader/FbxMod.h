/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#include "GlFunctions.h"

class CFbxMod
{
public:
	enum Status
    {
        UNLOADED,               // Unload file or load failure;
        MUST_BE_LOADED,         // Ready for loading file;
        MUST_BE_REFRESHED,      // Something changed and redraw needed;
        REFRESHED               // No redraw needed.
    };
    Status GetStatus() const { return mStatus; }

private:
	const char* mFileName;

	GLuint* mAnimationList;
	GLuint mCurrentList;
	GLuint mFrameCount;
	GLfloat mAnimationRate;
	GLboolean mAnimationDone;
	GLboolean mDisplayList;

	FbxManager* mSdkManager;
    FbxScene* mScene;
    FbxImporter* mImporter;
    FbxAnimLayer* mCurrentAnimLayer;

	mutable Status mStatus;

	mutable FbxTime mFrameTime, mStart, mStop, mCurrentTime;
	mutable FbxTime mCache_Start, mCache_Stop;

	GLboolean mDisplayOnce;

	GLboolean mSupportVBO;

	FbxArray<FbxString*> mAnimStackNameArray;

public:
	CFbxMod(void);
	CFbxMod(const char* path);
	~CFbxMod(void);

	GLfloat GetCurrentFrame(void) { if(mDisplayList) return mCurrentList; else return mCurrentTime.GetMilliSeconds(); }
	GLfloat GetAnimationTime(void) { if(mDisplayList) return mFrameCount; else return (mStop - mStart).GetMilliSeconds(); }

	void SetCurrentFrame(GLfloat frame) { if(mDisplayList) mCurrentList = frame; else mCurrentTime = frame; }
	void ResetCurrentTime(void) { if(mDisplayList) mCurrentList = 0; else mCurrentTime = mStart; }
	void SetAnimationRate(GLfloat rate) { mAnimationRate = rate; }
	void ResetAnimationRate(void) { mAnimationRate = 1; }
	void SetDisplayOnce(GLboolean mode) { mDisplayOnce = mode; }
	GLboolean AnimationDone(void);

	GLboolean InitializeOpenGL(void);

	const FbxScene * GetScene() const { return mScene; }
	// The time period for one frame.
    const GLuint GetFrameTime() const { return mFrameTime.GetMilliSeconds() * mAnimationRate; }

	// Get all the animation stack names in current scene.
    const FbxArray<FbxString *> & GetAnimStackNameArray() const { return mAnimStackNameArray; }

	GLboolean LoadFile(void);
	GLboolean LoadTextureFromFile(const FbxString & pFilePath, unsigned int & pTextureObject);
	void LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName, GLboolean pSupportVBO);
	void UnloadCacheRecursive(FbxScene * pScene);
	void LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer, GLboolean pSupportVBO);
	void UnloadCacheRecursive(FbxNode * pNode);

	GLboolean SetCurrentAnimStack(int pIndex);
	GLboolean EnableDisplayList(void);

	GLboolean LoadMod(const char* path);
	GLboolean OnDisplay(void);
	void OnTimerClick(void);
};

