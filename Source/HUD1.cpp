// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD1.h"
#include "ThirdPersonShooter.h"
#include "Kismet/GameplayStatics.h"
#include "Avatar.h"

void AHUD1::DrawHUD()
{
	//Super::DrawHUD();
	////Now Draw your stuff on the HUD
	////for example a blue line
	//DrawLine(200, 300, 400, 500, FLinearColor::Blue);
	////or some text in the middle of the screen
	//float ScreenX = 0.0f;
	//float ScreenY = 0.0f;
	//float TextScale = 5.0f;
	//bool bDontScale = false;
	//DrawText("Hello World", FColor::White, ScreenX, ScreenY, hudFont, TextScale, bDontScale);

	Super::DrawHUD();
	DrawMessages();
	DrawHealthbar();
	DrawWidgets();
	DisplayAmmoCount();
	EnemiesRemainingCount();
	dims.X = Canvas->SizeX;
	dims.Y = Canvas->SizeY;

	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (avatar->EnemiesRemaining <= 0)
	{
		YouWin();
	}

	if (avatar->Hp <= 0)
	{
		GameOver();
	}
	
}

void AHUD1::addMessage(Message msg)
{
	messages.Add(msg);
}
void AHUD1::DrawMessages()
{
	// iterate from back to front thru the list, so if we remove
	// an item while iterating, there won't be any problems
	for (int c = messages.Num() - 1; c >= 0; c--)
	{
		// draw the background box the right size for the message
		float ow, oh, pad = 10.f;
		GetTextSize(messages[c].message, ow, oh, hudFont, 1.f);
		float messageH = oh + 2 * pad;
		float x = 0, y = c * messageH;
		float FontScale = 1.5f;
		DrawRect(FLinearColor::Black, x, y, Canvas-> SizeX, messageH);
		DrawText(messages[c].message, messages[c].frontColor, x + pad + 30.f, y + pad,
			hudFont, FontScale);
		DrawTexture(messages[c].tex, x, y, messageH, messageH, 0, 0, 1, 1);

		// reduce lifetime by the time that passed since last frame.
		messages[c].time -= GetWorld()->GetDeltaSeconds();
		// if the message's time is up, remove it
		if (messages[c].time < 0)
		{
			messages.RemoveAt(c);
		}
	}
}


void AHUD1::DisplayAmmoCount()
{
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	int currentAmmo = avatar->ammo;
	FString ammoText = FString::FromInt(currentAmmo);
	float TextScale = 2.5f;
	float ScreenX = 10.0f;
	float ScreenY = 500.0f;
	DrawText("Ammo: " + ammoText, FLinearColor::Black, ScreenX, ScreenY, hudFont, TextScale);
	
	
	
}



void AHUD1::DrawHealthbar()
{
	// Draw the healthbar.
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	float barWidth = 200, barHeight = 25, barPad = 12, barMargin = 50;
	float percHp = avatar->Hp / avatar->MaxHp;
	/*DrawRect(FLinearColor::Red, Canvas->SizeX - barWidth - barPad - barMargin,
		Canvas->SizeY - barHeight - barPad - barMargin, barWidth + 2 * barPad,
		barHeight + 2 * barPad);*/
	DrawRect(FLinearColor(1 - percHp, percHp, 0, 1),
		Canvas->SizeX - barWidth - barMargin,
		Canvas->SizeY - barHeight - barMargin, barWidth*percHp, barHeight);
	int currentHealth = avatar->Hp;
	FString healthText = FString::FromInt(currentHealth);
	float TextScale = 2.5f;
	float ScreenX = 970.0f;
	float ScreenY = 450.0f;
	DrawText("Health: " + healthText, FLinearColor(1 - percHp, percHp, 0, 1), ScreenX, ScreenY, hudFont, TextScale);

}

void AHUD1::EnemiesRemainingCount()
{
	AAvatar *avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	int EnemiesCount = avatar->EnemiesRemaining;
	FString enemyText = FString::FromInt(EnemiesCount);
	float TextScale = 2.5f;
	float ScreenX = 450.0f;
	float ScreenY = 50.0f;
	DrawText("Enemies Remaining: " + enemyText, FLinearColor::White, ScreenX, ScreenY, hudFont, TextScale);
}

void AHUD1::GameOver()
{
	float TextScale = 5.0f;
	float ScreenX = 450.0f;
	float ScreenY = 250.0f;
	DrawText("Game Over", FLinearColor::Red, ScreenX, ScreenY, hudFont, TextScale);
}

void AHUD1::YouWin()
{
	float TextScale = 5.0f;
	float ScreenX = 450.0f;
	float ScreenY = 250.0f;
	DrawText("You Win", FLinearColor::Yellow, ScreenX, ScreenY, hudFont, TextScale);
}

void AHUD1::DrawWidgets()
{
	for (int c = 0; c < widgets.Num(); c++)
	{
		DrawTexture(widgets[c].icon.tex, widgets[c].pos.Y, widgets[c].pos.X,
			widgets[c].size.X, widgets[c].size.Y, 0, 0, 1, 1);
		DrawText(widgets[c].icon.name, FLinearColor::Yellow, widgets[c].pos.Y, widgets[c].pos.X,
			hudFont, 2.0f, false);
	}
}

void AHUD1::clearWidgets()
{
	widgets.Empty();
}


void AHUD1::addWidget(Widget widget)
{
	// find the pos of the widget based on the grid.
	// draw the icons..
	FVector2D start(200, 200), pad(12, 12);
	widget.size = FVector2D(100, 100);
	widget.pos = start;
	// compute the position here
	for (int c = 0; c < widgets.Num(); c++)
	{
		// Move the position to the right a bit.
		widget.pos.X += widget.size.X + pad.X;
		// If there is no more room to the right then
		// jump to the next line
		if (widget.pos.X + widget.size.X > dims.X)
		{
			widget.pos.X = start.X;
			widget.pos.Y += widget.size.Y + pad.Y;
		}
	}
	widgets.Add(widget);
}

void AHUD1::MouseClicked()
{
	FVector2D mouse;
	APlayerController *PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(mouse.X, mouse.Y);
	for (int c = 0; c < widgets.Num(); c++)
	{
		if (widgets[c].hit(mouse))
		{
			heldWidget = &widgets[c];
			return;
		}
	}
}

void AHUD1::MouseMoved()
{
	APlayerController *PController = GetWorld()->GetFirstPlayerController();
	float time = PController->GetInputKeyTimeDown(EKeys::LeftMouseButton);
	static FVector2D lastMouse;
	FVector2D thisMouse, dMouse;
	PController->GetMousePosition(thisMouse.Y, thisMouse.X);
	dMouse = thisMouse - lastMouse;
	if (time > 0.f && heldWidget)
	{
		// the mouse is being held down.
		// move the widget by displacement amount
		heldWidget->pos.Y += dMouse.Y;
		heldWidget->pos.X += dMouse.X; 
	}
	lastMouse = thisMouse;
}




