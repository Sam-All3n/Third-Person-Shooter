// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "HUD1.generated.h"


struct Message
{
	FString message;
	float time;
	FColor frontColor;
	UTexture2D* tex;
	Message()
	{
		// Set the default time. time = 5.f;
		frontColor = FColor::White;
	}
	Message(UTexture2D* iTex, FString iMessage, float iTime, FColor iFrontColor)
	{
		tex = iTex;
		message = iMessage;
		time = iTime;
		frontColor = iFrontColor;
	}
};


struct Icon
{
	FString name; UTexture2D* tex;
	Icon() { name = "UNKNOWN ICON"; tex = 0; }
	Icon(FString& iName, UTexture2D* iTex)
	{
		name = iName;
		tex = iTex;
	}
};


struct Widget
{
	Icon icon;
	UClass *className;
	FVector2D pos, size;

	Widget(Icon iicon, UClass *iClassName)
	{
		icon = iicon;
		className = iClassName;
	}

	float left() { return pos.X; }
	float right() { return pos.X + size.X; }
	float top() { return pos.Y; }
	float bottom() { return pos.Y + size.Y; }

	bool hit(FVector2D v)
	{
		// +---+ top (0)
		// | |
		// +---+ bottom (2) (bottom > top)
		// L R
		return v.X > left() && v.X < right() && v.Y > top() && v.Y < bottom();
	}
};

UCLASS()
class THIRDPERSONSHOOTER_API AHUD1 : public AHUD
{
	GENERATED_BODY()
public:
	// The font used to render the text in the HUD.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
		UFont* hudFont;

	// Called every frame
	virtual void DrawHUD() override;
	// The array of messages available 
	TArray<Message> messages;
	void addMessage(Message msg); // a function to add a message to display
	void DrawMessages();
	void DrawHealthbar();
	void DrawWidgets();
	void clearWidgets();
	void addWidget(Widget widget);
	void MouseClicked();
	void MouseMoved();
	void DisplayAmmoCount();
	void EnemiesRemainingCount();
	void GameOver();
	void YouWin();

	//These are the dims of the screen
	FVector2D dims;
	//The array of widgets available
	TArray<Widget> widgets;

	Widget* heldWidget;
	
	
};