/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include <atlimage.h>
#include "Common.h"
#include "DrawScene.h"
#include "SceneCache.h"
#include "targa.h"
#include "GL\glut.h"
#include "FbxMod.h"

CFbxMod::CFbxMod(void)
: mSdkManager(NULL), mScene(NULL), mImporter(NULL), mCurrentAnimLayer(NULL), mCurrentList(0), mFrameCount(0), mAnimationDone(GL_FALSE), mAnimationRate(1.0), mDisplayList(GL_FALSE), mDisplayOnce(GL_FALSE)
{
}


CFbxMod::CFbxMod(const char* path) 
: mFileName(path), mSdkManager(NULL), mScene(NULL), mImporter(NULL), mCurrentAnimLayer(NULL), mCurrentList(0), mFrameCount(0), mAnimationDone(GL_FALSE), mAnimationRate(1.0), mDisplayList(GL_FALSE), mDisplayOnce(GL_FALSE)
{
	mSupportVBO = InitializeOpenGL();

	// initialize cache start and stop time
	mCache_Start = FBXSDK_TIME_INFINITE;
	mCache_Stop  = FBXSDK_TIME_MINUS_INFINITE;

   // Create the FBX SDK manager which is the object allocator for almost 
   // all the classes in the SDK and create the scene.
   InitializeSdkObjects(mSdkManager, mScene);

   if (mSdkManager)
   {
       // Create the importer.
       int lFileFormat = -1;
       mImporter = FbxImporter::Create(mSdkManager,"");

       // Initialize the importer by providing a filename.
       if(mImporter->Initialize(mFileName, lFileFormat) == true)
       {
           mStatus = MUST_BE_LOADED;
       }
   }

   LoadFile();

   int lCurrentAnimStackIndex = 0;

    // Add the animation stack names.
    const FbxArray<FbxString *> & lAnimStackNameArray = GetAnimStackNameArray();
    for (int lPoseIndex = 0; lPoseIndex < lAnimStackNameArray.GetCount(); ++lPoseIndex)
    {
        // Track the current animation stack index.
        if (lAnimStackNameArray[lPoseIndex]->Compare(GetScene()->ActiveAnimStackName.Get()) == 0)
        {
            lCurrentAnimStackIndex = lPoseIndex;
        }
    }
	SetCurrentAnimStack(lCurrentAnimStackIndex);
}


CFbxMod::~CFbxMod(void)
{
	FbxArrayDelete(mAnimStackNameArray);

    // Unload the cache and free the memory
    if (mScene)
    {
        UnloadCacheRecursive(mScene);
    }

    // Delete the FBX SDK manager. All the objects that have been allocated 
    // using the FBX SDK manager and that haven't been explicitly destroyed 
    // are automatically destroyed at the same time.
	DestroySdkObjects(mSdkManager, true);
}


GLboolean CFbxMod::InitializeOpenGL(void)
{
	// Initialize GLEW.
    GLenum lError = glewInit();
    if (lError != GLEW_OK)
    {
        FBXSDK_printf("GLEW Error: %s\n", glewGetErrorString(lError));
        return GL_FALSE;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // OpenGL 1.5 at least.
    if (!GLEW_VERSION_1_5)
    {
        FBXSDK_printf("The OpenGL version should be at least 1.5 to display shaded scene!\n");
        return GL_FALSE;
    }

    return TRUE;
}


GLboolean CFbxMod::LoadMod(const char* path)
{
	mFileName = path;

	mSupportVBO = InitializeOpenGL();

	// initialize cache start and stop time
	mCache_Start = FBXSDK_TIME_INFINITE;
	mCache_Stop  = FBXSDK_TIME_MINUS_INFINITE;

	// Create the FBX SDK manager which is the object allocator for almost 
	// all the classes in the SDK and create the scene.
	InitializeSdkObjects(mSdkManager, mScene);

	if (mSdkManager)
	{
		// Create the importer.
		int lFileFormat = -1;
		mImporter = FbxImporter::Create(mSdkManager,"");

		// Initialize the importer by providing a filename.
		if(mImporter->Initialize(mFileName, lFileFormat) == true)
		{
			mStatus = MUST_BE_LOADED;
		}
	}

	LoadFile();

	int lCurrentAnimStackIndex = 0;

    // Add the animation stack names.
    const FbxArray<FbxString *> & lAnimStackNameArray = GetAnimStackNameArray();
    for (int lPoseIndex = 0; lPoseIndex < lAnimStackNameArray.GetCount(); ++lPoseIndex)
    {
        // Track the current animation stack index.
        if (lAnimStackNameArray[lPoseIndex]->Compare(GetScene()->ActiveAnimStackName.Get()) == 0)
        {
            lCurrentAnimStackIndex = lPoseIndex;
        }
    }
	SetCurrentAnimStack(lCurrentAnimStackIndex);

	return GL_TRUE;
}


GLboolean CFbxMod::LoadFile(void)
{
	GLboolean lResult = false;
    // Make sure that the scene is ready to load.
    if (mStatus == MUST_BE_LOADED)
    {
        if (mImporter->Import(mScene) == true)
        {
            // Set the scene status flag to refresh 
            // the scene in the first timer callback.
            mStatus = MUST_BE_REFRESHED;

            // Convert Axis System to what is used in this example, if needed
            FbxAxisSystem SceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();
            FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
            if( SceneAxisSystem != OurAxisSystem )
            {
                OurAxisSystem.ConvertScene(mScene);
            }

            // Convert Unit System to what is used in this example, if needed
            FbxSystemUnit SceneSystemUnit = mScene->GetGlobalSettings().GetSystemUnit();
            if( SceneSystemUnit.GetScaleFactor() != 1.0 )
            {
                //The unit in this example is centimeter.
                FbxSystemUnit::cm.ConvertScene( mScene);
            }

            // Get the list of all the animation stack.
            mScene->FillAnimStackNameArray(mAnimStackNameArray);

            // Convert mesh, NURBS and patch into triangle mesh
			FbxGeometryConverter lGeomConverter(mSdkManager);
			lGeomConverter.Triangulate(mScene, /*replace*/true);

			// Split meshes per material, so that we only have one material per mesh (for VBO support)
			lGeomConverter.SplitMeshesPerMaterial(mScene, /*replace*/true);

            // Bake the scene for one frame
            LoadCacheRecursive(mScene, mCurrentAnimLayer, mFileName, mSupportVBO);

            // Initialize the frame period.
            mFrameTime.SetTime(0, 0, 0, 1, 0, mScene->GetGlobalSettings().GetTimeMode());

            lResult = GL_TRUE;
        }
        else
        {
            // Import failed, set the scene status flag accordingly.
            mStatus = UNLOADED;
        }
        // Destroy the importer to release the file.
        mImporter->Destroy();
        mImporter = NULL;
    }

    return lResult;
}


// Load a texture file (TGA only now) into GPU and return the texture object name
GLboolean CFbxMod::LoadTextureFromFile(const FbxString & pFilePath, unsigned int & pTextureObject)
{
    if (pFilePath.Right(3).Upper() == "TGA")
    {
        tga_image lTGAImage;

        if (tga_read(&lTGAImage, pFilePath.Buffer()) == TGA_NOERR)
        {
            // Make sure the image is left to right
            if (tga_is_right_to_left(&lTGAImage))
                tga_flip_horiz(&lTGAImage);

            // Make sure the image is bottom to top
            if (tga_is_top_to_bottom(&lTGAImage))
                tga_flip_vert(&lTGAImage);

            // Make the image BGR 24
            tga_convert_depth(&lTGAImage, 24);

            // Transfer the texture date into GPU
            glGenTextures(1, &pTextureObject);
            glBindTexture(GL_TEXTURE_2D, pTextureObject);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, lTGAImage.width, lTGAImage.height, 0, GL_BGR,
                GL_UNSIGNED_BYTE, lTGAImage.image_data);
            glBindTexture(GL_TEXTURE_2D, 0);

            tga_free_buffers(&lTGAImage);

            return true;
        }
    }

    return false;
}


// Bake node attributes and materials for this scene and load the textures.
void CFbxMod::LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName, GLboolean pSupportVBO)
{
    // Load the textures into GPU, only for file texture now
    const int lTextureCount = pScene->GetTextureCount();
    for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
    {
        FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
        FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
        if (lFileTexture && !lFileTexture->GetUserDataPtr())
        {
            // Try to load the texture from absolute path
            const FbxString lFileName = lFileTexture->GetFileName();
                
            // Only TGA textures are supported now.
            if (lFileName.Right(3).Upper() != "TGA")
            {
                FBXSDK_printf("Only TGA textures are supported now: %s\n", lFileName.Buffer());
                continue;
            }

            GLuint lTextureObject = 0;
            GLboolean lStatus = LoadTextureFromFile(lFileName, lTextureObject);

            const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName);
            const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);
            if (!lStatus)
            {
                // Load texture from relative file name (relative to FBX file)
                const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
                lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
            }

            if (!lStatus)
            {
                // Load texture from file name only (relative to FBX file)
                const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
                const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
                lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
            }

            if (!lStatus)
            {
                FBXSDK_printf("Failed to load texture file: %s\n", lFileName.Buffer());
                continue;
            }

            if (lStatus)
            {
                GLuint * lTextureName = new GLuint(lTextureObject);
                lFileTexture->SetUserDataPtr(lTextureName);
            }
        }
    }

    LoadCacheRecursive(pScene->GetRootNode(), pAnimLayer, pSupportVBO);
}

// Unload the cache and release the memory fro this scene and release the textures in GPU
void CFbxMod::UnloadCacheRecursive(FbxScene * pScene)
{
    const int lTextureCount = pScene->GetTextureCount();
    for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
    {
        FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
        FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
        if (lFileTexture && lFileTexture->GetUserDataPtr())
        {
            GLuint * lTextureName = static_cast<GLuint *>(lFileTexture->GetUserDataPtr());
            lFileTexture->SetUserDataPtr(NULL);
            glDeleteTextures(1, lTextureName);
            delete lTextureName;
        }
    }

    UnloadCacheRecursive(pScene->GetRootNode());
}


// Bake node attributes and materials under this node recursively.
// Currently only mesh, light and material.
void CFbxMod::LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer, GLboolean pSupportVBO)
{
    // Bake material and hook as user data.
    const int lMaterialCount = pNode->GetMaterialCount();
    for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
    {
        FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
        if (lMaterial && !lMaterial->GetUserDataPtr())
        {
            FbxAutoPtr<MaterialCache> lMaterialCache(new MaterialCache);
            if (lMaterialCache->Initialize(lMaterial))
            {
                lMaterial->SetUserDataPtr(lMaterialCache.Release());
            }
        }
    }

    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
    if (lNodeAttribute)
    {
        // Bake mesh as VBO(vertex buffer object) into GPU.
        if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh * lMesh = pNode->GetMesh();
            if (pSupportVBO && lMesh && !lMesh->GetUserDataPtr())
            {
                FbxAutoPtr<VBOMesh> lMeshCache(new VBOMesh);
                if (lMeshCache->Initialize(lMesh))
                {
                    lMesh->SetUserDataPtr(lMeshCache.Release());
                }
            }
        }
        // Bake light properties.
        else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
        {
            FbxLight * lLight = pNode->GetLight();
            if (lLight && !lLight->GetUserDataPtr())
            {
                FbxAutoPtr<LightCache> lLightCache(new LightCache);
                if (lLightCache->Initialize(lLight, pAnimLayer))
                {
                    lLight->SetUserDataPtr(lLightCache.Release());
                }
            }
        }
    }

    const int lChildCount = pNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
    {
        LoadCacheRecursive(pNode->GetChild(lChildIndex), pAnimLayer, pSupportVBO);
    }
}

// Unload the cache and release the memory under this node recursively.
void CFbxMod::UnloadCacheRecursive(FbxNode * pNode)
{
    // Unload the material cache
    const int lMaterialCount = pNode->GetMaterialCount();
    for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
    {
        FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
        if (lMaterial && lMaterial->GetUserDataPtr())
        {
            MaterialCache * lMaterialCache = static_cast<MaterialCache *>(lMaterial->GetUserDataPtr());
            lMaterial->SetUserDataPtr(NULL);
            delete lMaterialCache;
        }
    }

    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
    if (lNodeAttribute)
    {
        // Unload the mesh cache
        if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh * lMesh = pNode->GetMesh();
            if (lMesh && lMesh->GetUserDataPtr())
            {
                VBOMesh * lMeshCache = static_cast<VBOMesh *>(lMesh->GetUserDataPtr());
                lMesh->SetUserDataPtr(NULL);
                delete lMeshCache;
            }
        }
        // Unload the light cache
        else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
        {
            FbxLight * lLight = pNode->GetLight();
            if (lLight && lLight->GetUserDataPtr())
            {
                LightCache * lLightCache = static_cast<LightCache *>(lLight->GetUserDataPtr());
                lLight->SetUserDataPtr(NULL);
                delete lLightCache;
            }
        }
    }

    const int lChildCount = pNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
    {
        UnloadCacheRecursive(pNode->GetChild(lChildIndex));
    }
}


GLboolean CFbxMod::SetCurrentAnimStack(int pIndex)
{
	const int lAnimStackCount = mAnimStackNameArray.GetCount();
    if (!lAnimStackCount || pIndex >= lAnimStackCount)
    {
        return false;
    }

    // select the base layer from the animation stack
   FbxAnimStack * lCurrentAnimationStack = mScene->FindMember<FbxAnimStack>(mAnimStackNameArray[pIndex]->Buffer());
   if (lCurrentAnimationStack == NULL)
   {
       // this is a problem. The anim stack should be found in the scene!
       return false;
   }

   // we assume that the first animation layer connected to the animation stack is the base layer
   // (this is the assumption made in the FBXSDK)
   mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
   mScene->SetCurrentAnimationStack(lCurrentAnimationStack);

   FbxTakeInfo* lCurrentTakeInfo = mScene->GetTakeInfo(*(mAnimStackNameArray[pIndex]));
   if (lCurrentTakeInfo)
   {
       mStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
       mStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
   }
   else
   {
       // Take the time line value
       FbxTimeSpan lTimeLineTimeSpan;
       mScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

       mStart = lTimeLineTimeSpan.GetStart();
       mStop  = lTimeLineTimeSpan.GetStop();
   }
   
   // check for smallest start with cache start
   if(mCache_Start < mStart)
	   mStart = mCache_Start;

   // check for biggest stop with cache stop
   if(mCache_Stop  > mStop)  
	   mStop  = mCache_Stop;

   // move to beginning
   mCurrentTime = mStart;

   // Set the scene status flag to refresh 
   // the scene in the next timer callback.
   mStatus = MUST_BE_REFRESHED;

   return GL_TRUE;
}


GLboolean CFbxMod::EnableDisplayList(void)
{
	while(!(mCurrentTime > mStop))
	{
		mCurrentTime += mFrameTime;
		mFrameCount++;
	}
	mCurrentTime = mStart;

	mAnimationList = new GLuint[mFrameCount];

	for(GLuint i = 0; i < mFrameCount; i++)
	{
		mAnimationList[i] = glGenLists(1);
		glNewList(mAnimationList[i], GL_COMPILE);
			glPushAttrib(GL_ENABLE_BIT);
			glPushAttrib(GL_LIGHTING_BIT);
			glEnable(GL_DEPTH_TEST);
			// Draw the front face only, except for the texts and lights.
			glEnable(GL_CULL_FACE);

			FbxPose * lPose = NULL;
			FbxAMatrix lDummyGlobalPosition;
        
			//InitializeLights(mScene, mCurrentTime, lPose);
			DrawNodeRecursive(mScene->GetRootNode(), mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, SHADING_MODE_SHADED);

			glPopAttrib();
			glPopAttrib();
		glEndList();

		mCurrentTime += mFrameTime;
	}
	mDisplayList = GL_TRUE;

	return GL_TRUE;
}


GLboolean CFbxMod::OnDisplay(void)
{
	switch(mDisplayList) {
	case GL_TRUE: 
		glCallList(mAnimationList[mCurrentList]);
		return GL_TRUE;
		break;

	case GL_FALSE:
		glPushAttrib(GL_ENABLE_BIT);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_DEPTH_TEST);
		// Draw the front face only, except for the texts and lights.
		glEnable(GL_CULL_FACE);

		FbxPose * lPose = NULL;
		FbxAMatrix lDummyGlobalPosition;
        
		//InitializeLights(mScene, mCurrentTime, lPose);
		DrawNodeRecursive(mScene->GetRootNode(), mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, SHADING_MODE_SHADED);

		glPopAttrib();
		glPopAttrib();
		
		return GL_TRUE;
		break;
	}
}


void CFbxMod::OnTimerClick(void)
{
	switch(mDisplayList) {
	case GL_TRUE:
		if(mCurrentList < mFrameCount-1) {
			if(mDisplayOnce && mAnimationDone)
				break;
		mAnimationDone = GL_FALSE;
		mCurrentList++;
		}
		else {
			mAnimationDone = GL_TRUE;
			if(!mDisplayOnce)
				mCurrentList = 0;
		}
		break;

	case GL_FALSE: 
		if (mStop > mStart)
		{
			// Set the scene status flag to refresh 
			// the scene in the next timer callback.
			mStatus = MUST_BE_REFRESHED;

			if(mDisplayOnce && mAnimationDone)
				break;
			mAnimationDone = GL_FALSE;
			mCurrentTime += mFrameTime;

			if (mCurrentTime > mStop)
			{
				mAnimationDone = GL_TRUE;
				if(!mDisplayOnce)
					mCurrentTime = mStart;
			}
		}
		break;
	}
}


GLboolean CFbxMod::AnimationDone(void)
{
	if(mAnimationDone) {
		mAnimationDone = GL_FALSE; 
		return !mAnimationDone; 
	} 
	else 
		return mAnimationDone; 
}
