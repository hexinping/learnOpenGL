
#include <vector>
#include <string>
using namespace std;

#include "OpenglState.h"

vector<string> _nameArray = {
	"OpenglStateTriangle",
	"OpenglStateRect",
	"OpenglStateMultAttr",
	"OpenglStateTriangleMove",
	"OpenglStateTexture",
	"OpenglStateMultTexture",
	"OpenglStateMultTextureMat4",
	"OpenglStateMultTexture3D",
	"OpenglStateMultTextureCube",
	"OpenglStateMultTextureCamera",
	"OpenglStateMultTextureCameraManuel",
	"OpenglStateMultTextureLight",
	"OpenglStateMultTextureMaterial",
	"OpenglStateMultTextureMaterialMap",
	"OpenglStateMultTextureMaterialMapPointLight",
	"OpenglStateMultTextureMaterialMapDirLight",
	"OpenglStateMultTextureMaterialMapSpotLight",
	"OpenglStateMultTextureMaterialMapMultLights",
	"OpenglStateModel3D",
	"OpenglStateMultTextureDepthCube",
	"OpenglStateMultTextureBlend",
	"OpenglStateMultTextureTransparent",
	"OpenglStateReflect",
	"OpenglStateInStance",
	"OpenglStateInstance2",
	"OpenglStateModel3DPlanet",
	"OpenglStateModel3DRock",
	"OpenglStateMultTextureMultSample",
	"OpenglStateMultTextureMaterialMapMultLightsAdvance",
	"OpenglStateMultTextureMaterialMapPointLightAdvance",
	"OpenglStateNormalMap",
	"OpenglStateNormalMap1",
	"OpenglStateModel3DNormalMap",
	"OpenglStateHDR",
	"OpenglStateHDRBloom",
	"OpenglStateDelayRenderLights",
	"OpenglStateShadowDirLight",
	"OpenglStateShadowPointLight",
	"OpenglStateLabel",
	"OpenglStateMultTextureWave",
	"OpenglStateSSAO",
	"OpenglStatePBRSimple",
	"OpenglStatePBRTexture",
	"OpenglStatePBR_IBL_Irradiance_Conversion",
	"OpenglStatePBR_IBL_Irradiance",
	"OpenglStatePBR_IBL_Irradiance_Specular",
	"OpenglStatePBR_IBL_Irradiance_Specular_Texture",
};

OpenglState * getGLState(int curIndex)
 {

	 //OpenglState *glState = _classArray[curIndex];
	 //string name = _nameArray[curIndex];
	 //return glState;

	 OpenglState *glState = nullptr;
	 if (curIndex == 0)
	 {
		 glState = new OpenglStateTriangle();
	 }
	 else if (curIndex == 1)
	 {
		 glState = new OpenglStateRect();
	 }
	 else if (curIndex == 2)
	 {
		 glState = new OpenglStateMultAttr();
	 }
	 else if (curIndex == 3)
	 {
		 glState = new OpenglStateTriangleMove();
	 }
	 else if (curIndex == 4)
	 {
		 glState = new OpenglStateTexture();
	 }
	 else if (curIndex == 5)
	 {
		 glState = new OpenglStateMultTexture();
	 }
	 else if (curIndex == 6)
	 {
		 glState = new OpenglStateMultTextureMat4();
	 }
	 else if (curIndex == 7)
	 {
		 glState = new OpenglStateMultTexture3D();
	 }
	 else if (curIndex == 8)
	 {
		 glState = new OpenglStateMultTextureCube();
	 }
	 else if (curIndex == 9)
	 {
		 glState = new OpenglStateMultTextureCamera();
	 }
	 else if (curIndex == 10)
	 {
		 glState = new OpenglStateMultTextureCameraManuel();
	 }
	 else if (curIndex == 11)
	 {
		 glState = new OpenglStateMultTextureLight();
	 }
	 else if (curIndex == 12)
	 {
		 glState = new OpenglStateMultTextureMaterial();
	 }
	 else if (curIndex == 13)
	 {
		 glState = new OpenglStateMultTextureMaterialMap();
	 }
	 else if (curIndex == 14)
	 {
		 glState = new OpenglStateMultTextureMaterialMapPointLight();
	 }
	 else if (curIndex == 15)
	 {
		 glState = new OpenglStateMultTextureMaterialMapDirLight();
	 }
	 else if (curIndex == 16)
	 {
		 glState = new OpenglStateMultTextureMaterialMapSpotLight();
	 }
	 else if (curIndex == 17)
	 {
		 glState = new OpenglStateMultTextureMaterialMapMultLights();
	 }
	 else if (curIndex == 18)
	 {
		 glState = new OpenglStateModel3D();
	 }
	 else if (curIndex == 19)
	 {
		 glState = new OpenglStateMultTextureDepthCube();
	 }
	 else if (curIndex == 20)
	 {
		 glState = new OpenglStateMultTextureBlend();
	 }
	 else if (curIndex == 21)
	 {
		 glState = new OpenglStateMultTextureTransparent();
	 }
	 else if (curIndex == 22)
	 {
		 glState = new OpenglStateReflect();
	 }
	 else if (curIndex == 23)
	 {
		 glState = new OpenglStateInStance();
	 }
	 else if (curIndex == 24)
	 {
		 glState = new OpenglStateInstance2();
	 }
	 else if (curIndex == 25)
	 {
		 glState = new OpenglStateModel3DPlanet();
	 }
	 else if (curIndex == 26)
	 {
		 glState = new OpenglStateModel3DRock();
	 }
	 else if (curIndex == 27)
	 {
		 glState = new OpenglStateMultTextureMultSample();
	 }
	 else if (curIndex == 28)
	 {
		 glState = new OpenglStateMultTextureMaterialMapMultLightsAdvance();
	 }
	 else if (curIndex == 29)
	 {
		 glState = new OpenglStateMultTextureMaterialMapPointLightAdvance();
	 }
	 else if (curIndex == 30)
	 {
		 glState = new OpenglStateNormalMap();
	 }
	 else if (curIndex == 31)
	 {
		 glState = new OpenglStateNormalMap1();
	 }
	 else if (curIndex == 32)
	 {
		 glState = new OpenglStateModel3DNormalMap();
	 }
	 else if (curIndex == 33)
	 {
		 glState = new OpenglStateHDR();
	 }
	 else if (curIndex == 34)
	 {
		 glState = new OpenglStateHDRBloom();
	 }
	 else if (curIndex == 35)
	 {
		 glState = new OpenglStateDelayRenderLights();
	 }
	 else if (curIndex == 36)
	 {
		 glState = new OpenglStateShadowDirLight();
	 }
	 else if (curIndex == 37)
	 {
		 glState = new OpenglStateShadowPointLight();
	 }
	 else if (curIndex == 38)
	 {
		 glState = new OpenglStateLabel();
	 }
	 else if (curIndex == 39)
	 {
		 glState = new OpenglStateMultTextureWave();
	 }
	 else if (curIndex == 40)
	 {
		 glState = new OpenglStateSSAO();
	 }
	 else if (curIndex == 41)
	 {
		 glState = new OpenglStatePBRSimple();
	 }
	 else if (curIndex == 42)
	 {
		 glState = new OpenglStatePBRTexture();
	 }
	 else if (curIndex == 43)
	 {
		 glState = new OpenglStatePBR_IBL_Irradiance_Conversion();
	 }
	 else if (curIndex == 44)
	 {
		 glState = new OpenglStatePBR_IBL_Irradiance();
	 }
	 else if (curIndex == 45)
	 {
		 glState = new OpenglStatePBR_IBL_Irradiance_Specular();
	 }
	 else if (curIndex == 46)
	 {
		 glState = new OpenglStatePBR_IBL_Irradiance_Specular_Texture();
	 }

	 return glState;
 }

int getClassCount()
{
	return _nameArray.size();
}

string getClassName(int curIndex)
{
	return _nameArray[curIndex];
}