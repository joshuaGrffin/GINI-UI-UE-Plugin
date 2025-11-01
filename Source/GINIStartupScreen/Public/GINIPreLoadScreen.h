// The Griffin Initiative

#pragma once

#include "CoreMinimal.h"
#include "PreLoadScreenBase.h"

class SWidget;
/**
 * 
 */
class GINISTARTUPSCREEN_API FGINIPreLoadScreen : public FPreLoadScreenBase
{
public:
	virtual void Init() override;
	virtual EPreLoadScreenTypes GetPreLoadScreenType() const override{return EPreLoadScreenTypes::EngineLoadingScreen;}
	virtual  TSharedPtr<SWidget> GetWidget() override {return PreLoadScreen;}

private:
	TSharedPtr<SWidget> PreLoadScreen;
};
