#include "Framework.h"
MCategory* MCategory::CurrentCategory = nullptr;
MTab * MTab::CurrentTab = nullptr;
bool waiting;
bool isKeyPressing;
int tick;
bool isKeyPressed2;
char nuts;
template< class T, class Y >

T Clamp(T const &val, Y const &minVal, Y const &maxVal)
{
	if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}

static bool ToggleButton(ButtonCode_t code)
{
	static int buttonPressedTick = 0;    
	if (InputSystem->IsButtonDown(code) && (GetTickCount64() - buttonPressedTick) > 300)
	{
		buttonPressedTick = GetTickCount64();
		return true;
	}
	return false;
}

bool Clicked(Vector2D position, Vector2D bounds)
{
	POINT Cur;
	GetCursorPos(&Cur);
	return (Cur.x > position.x && Cur.y > position.y &&
		Cur.x < position.x + bounds.x && Cur.y < position.y + bounds.y &&
		ToggleButton(MOUSE_LEFT));
}

bool InBounds(Vector2D position, Vector2D bounds)
{
	POINT Cur;
	GetCursorPos(&Cur);
	return (Cur.x > position.x && Cur.y > position.y &&
		Cur.x < position.x + bounds.x && Cur.y < position.y + bounds.y);
}

MWindow::MWindow(std::string name, Vector2D pos, Vector2D size)
{
	this->name = name;
	this->position = pos;
	this->size = size;
	this->visible = true;
}

void MWindow::AddChildControl(MBaseObject* Child)
{
	Children.push_back(Child);
}

//MenuBackground
void MWindow::Draw()
{

	if (this->visible)
	{
		static float rainbow;
		DrawRect(this->position.x, this->position.y, this->size.x, this->size.y, Color(15, 15, 15, 255));//entire black fill
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, this->size.x + 1, this->size.y + 1, Color(15, 15, 15, 255));//inner black line
		DrawOutlinedRect(this->position.x - 2, this->position.y - 2, this->size.x + 3, this->size.y + 3, Color(60, 60, 60, 255));//inner light gray line
		DrawOutlinedRect(this->position.x - 3, this->position.y - 3, this->size.x + 5, this->size.y + 5, Color(40, 40, 40, 255));//1px 3.1 line connection
		DrawOutlinedRect(this->position.x - 4, this->position.y - 4, this->size.x + 7, this->size.y + 7, Color(40, 40, 40, 255));//1px 3.2 line connection
		DrawOutlinedRect(this->position.x - 5, this->position.y - 5, this->size.x + 9, this->size.y + 9, Color(40, 40, 40, 255));//1px 3.3 line connection
		DrawOutlinedRect(this->position.x - 6, this->position.y - 6, this->size.x + 11, this->size.y + 11, Color(60, 60, 60, 255));//outer light gray line
		DrawOutlinedRect(this->position.x - 7, this->position.y - 7, this->size.x + 13, this->size.y + 13, Color(15, 15, 15, 255));//outer black line
		DrawRectRainbow(this->position.x, this->position.y, this->size.x - 1, 3, 0.002, rainbow);
		for (int i = 0; i < Children.size(); i++)
		{
			Children[i]->Draw();
		}
	}
}

MCategory::MCategory(std::string name, Vector2D pos, MWindow* parent)
{
	this->name = name;
	this->position = pos;
	this->parent = parent;
}

void MCategory::AddChildControl(MBaseObject* Child)
{
	Children.push_back(Child);
}

void MCategory::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y, 100, 20))
	{
		//MCategory::CurrentCategory->active = false;
		this->active = true;
		MCategory::CurrentCategory = this;
	}

	if (this->active)
	{
		DrawString(Impact, this->position.x + 20, this->position.y, Color::White(), FONT_LEFT, "%s", this->name.c_str());
		for (int i = 0; i < Children.size(); i++) Children[i]->Draw();
	}
	else
	{
		DrawString(Impact, this->position.x + 20, this->position.y, Color(80, 80, 80, 255), FONT_LEFT, "%s", this->name.c_str());
	}
}

MButton::MButton(std::string name, Vector2D pos, Vector2D size)
{
	this->name = name;
	this->position = pos;
	this->size = size;
}

void MButton::Draw()
{

	if (Input->Hovering(position.x, position.y, size.x, size.y))
	{
		DrawRectGradientVertical(position.x, position.y, size.x, size.y, Color(50, 50, 50, 255), Color(30, 30, 30, 255));
		DrawOutlinedRect(position.x - 1, position.y - 1, size.x + 2, size.y + 2, Color(50, 50, 50, 255));
		DrawOutlinedRect(position.x - 2, position.y - 2, size.x + 4, size.y + 4, Color(0, 0, 0, 255));
		DrawString(Verdana, position.x + size.x / 2, position.y + size.y / 2, Color(164, 164, 164, 255), FONT_CENTER, name.c_str());
		DrawRect(position.x - 2, position.y - 2, size.x + 5, size.y + 5, Color(255, 255, 255, 2));
	}
	else
	{
		DrawRectGradientVertical(position.x, position.y, size.x, size.y, Color(50, 50, 50, 255), Color(30, 30, 30, 255));
		DrawOutlinedRect(position.x - 1, position.y - 1, size.x + 2, size.y + 2, Color(50, 50, 50, 255));
		DrawOutlinedRect(position.x - 2, position.y - 2, size.x + 4, size.y + 4, Color(0, 0, 0, 255));
		DrawString(Verdana, position.x + size.x / 2, position.y + size.y / 2, Color(164, 164, 164, 255), FONT_CENTER, name.c_str());
	}
}

struct CurrentPlayer
{
	char* name;
} selectedPlayer[64];

MButtonGroup::MButtonGroup(MGroup* parent, std::string name, Vector2D pos, Vector2D size)
{
	this->parent = parent;
	this->name = name;
	this->position = pos;
	this->size = size;
}

void MButtonGroup::Draw()
{

	if (Input->Hovering(position.x, position.y, size.x, size.y))
	{
		DrawRectGradientVertical(position.x, position.y, size.x, size.y, Color(50, 50, 50, 255), Color(30, 30, 30, 255));
		DrawOutlinedRect(position.x - 1, position.y - 1, size.x + 2, size.y + 2, Color(50, 50, 50, 255));
		DrawOutlinedRect(position.x - 2, position.y - 2, size.x + 4, size.y + 4, Color(0, 0, 0, 255));
		DrawString(Verdana, position.x + size.x / 2, position.y + size.y / 2, Color(164, 164, 164, 255), FONT_CENTER, name.c_str());
		DrawRect(position.x - 2, position.y - 2, size.x + 5, size.y + 5, Color(255, 255, 255, 2));
	}
	else
	{
		DrawRectGradientVertical(position.x, position.y, size.x, size.y, Color(50, 50, 50, 255), Color(30, 30, 30, 255));
		DrawOutlinedRect(position.x - 1, position.y - 1, size.x + 2, size.y + 2, Color(50, 50, 50, 255));
		DrawOutlinedRect(position.x - 2, position.y - 2, size.x + 4, size.y + 4, Color(0, 0, 0, 255));
		DrawString(Verdana, position.x + size.x / 2, position.y + size.y / 2, Color(164, 164, 164, 255), FONT_CENTER, name.c_str());
	}
}

MString::MString(MGroup* parent, std::string name, Vector2D pos)
{
	this->parent = parent;
	this->name = name;
	this->position = pos;
}

void MString::Draw()
{
	DrawString(Verdana, this->position.x, this->position.y, Color(164, 164, 164, 255), FONT_CENTER, name.c_str());
}

MGroup::MGroup(MCategory* category, std::string name, Vector2D pos, Vector2D size)
{
	this->category = category;
	this->name = name;
	this->position = pos;
	this->size = size;
}

void MGroup::AddChildControl(MBaseObject* Child)
{
	Children.push_back(Child);
}

//GroupLinesAndBoxes
void MGroup::Draw()
{
	//i really am lazy and dont give a FUCK
	DrawOutlinedRect(this->position.x, this->position.y, this->size.x, this->size.y, Color(50, 50, 50, 255)); //inner gray
	DrawOutlinedRect(this->position.x - 1, this->position.y - 1, this->size.x + 2, this->size.y + 2, Color::Black()); //outer black
																													  // + 1 or + 2?
																													  //watch this lol
																													  //who needs lines?
	DrawRect(this->position.x + 10, this->position.y - 2, GetStringWidth(Verdana, name.c_str()) + 10, 12, Color(15, 15, 15, 255));
	DrawString(Verdana, this->position.x + 15, this->position.y - 1, Color(200, 200, 200, 255), FONT_LEFT, name.c_str());
	for (int i = 0; i < Children.size(); i++) Children[i]->Draw();
	//for (int i = 0; i < Children.size(); i++) Children[i]->InputBox();
}

MCheckbox::MCheckbox(MGroup* parent, std::string name, bool* variable, Vector2D pos)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->position = pos;
}

void MCheckbox::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y, 8, 8))
		if (*this->Var == false)
			*this->Var = true;
		else
			*this->Var = false;

	POINT Cur; GetCursorPos(&Cur);
	std::wstring c = std::wstring(caption.begin(), caption.end());
	int textW, textH;
	Surface->GetTextSize(Verdana, c.c_str(), textW, textH);

	if (*this->Var)
	{
		DrawRectGradientVertical(this->position.x, this->position.y, 8, 8, Color(37, 195, 219,255), Color(37, 195, 219,255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, 9, 9, Color::Black());
	}
	else
	{
		DrawRectGradientVertical(this->position.x, this->position.y, 8, 8, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, 9, 9, Color::Black());
	}
	DrawString(Verdana, this->position.x + 13, this->position.y + 2, Color(201, 201, 201, 255), FONT_LEFT, name.c_str());
}

MCheckboxCategory::MCheckboxCategory(MCategory* parent, std::string name, bool* variable, Vector2D pos)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->position = pos;
}

void MCheckboxCategory::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y, 8, 8))
		if (*this->Var == false)
			*this->Var = true;
		else
			*this->Var = false;

	POINT Cur; GetCursorPos(&Cur);
	std::wstring c = std::wstring(caption.begin(), caption.end());
	int textW, textH;
	Surface->GetTextSize(Verdana, c.c_str(), textW, textH);

	if (*this->Var)
	{
		DrawRectGradientVertical(this->position.x, this->position.y, 8, 8, Color(37, 195, 219,255), Color(37, 195, 219,255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, 9, 9, Color::Black());
	}
	else
	{
		DrawRectGradientVertical(this->position.x, this->position.y, 8, 8, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, 9, 9, Color::Black());
	}
	DrawString(Verdana, this->position.x + 13, this->position.y + 2, Color(201, 201, 201, 255), FONT_LEFT, name.c_str());
}

MKey::MKey(MGroup* parent, std::string name, int* variable, Vector2D pos)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->position = pos;
}

void MKey::Draw()
{

	if (Input->Clicked(this->position.x, this->position.y, 8, 8))
			isKeyPressed2 = true;

	POINT Cur; GetCursorPos(&Cur);
	std::wstring c = std::wstring(caption.begin(), caption.end());
	int textW, textH;
	Surface->GetTextSize(Verdana, c.c_str(), textW, textH);

	if (isKeyPressed2)
	{
		DrawString(Verdana, this->position.x + 13, this->position.y + 2, Color(201, 201, 201, 255), FONT_LEFT, "Waiting...");
		for (int i = 1; i < 255; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				*this->Var = i;
				isKeyPressed2 = false;
			}
		}
	}
	else
	{
		if (*this->Var != 0)	
			DrawString(Verdana, this->position.x + 13, this->position.y + 2, Color(201, 201, 201, 255), FONT_LEFT, "%s: %i", name.c_str(), *this->Var);
		else
			DrawString(Verdana, this->position.x + 13, this->position.y + 2, Color(201, 201, 201, 255), FONT_LEFT, "%s", name.c_str());
	}

	if (*this->Var != 0)
	{
		DrawRectGradientVertical(this->position.x, this->position.y, 8, 8, Color(153, 50, 204, 255), Color(153, 50, 204, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, 9, 9, Color::Black());
	}
	else
	{
		DrawRectGradientVertical(this->position.x, this->position.y, 8, 8, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 1, 9, 9, Color::Black());
	}
}

MComboCategory::MComboCategory(MCategory* parent, std::string name, int* variable, Vector2D pos, std::vector<std::string> elements, bool* fuck)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->position = pos;
	this->elements = elements;
	this->IsOpen = fuck;
}

void MComboCategory::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y, 170, 20))
		if (*this->IsOpen == false)
			*this->IsOpen = true;
		else
			*this->IsOpen = false;

	if (elements.size() > 0)
	{
		DrawRectGradientVertical(this->position.x, this->position.y - 8, 170, 20, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 9, 172, 22, Color::Black());
		DrawString(Verdana, this->position.x + 11, this->position.y + 2, Color(255, 255, 255, 255), FONT_LEFT, this->elements[*this->Var].c_str());

		if (*this->IsOpen)
		{
			DrawRectGradientVertical(this->position.x, this->position.y + 14, 170, elements.size() * 16, Color(70, 70, 70, 255), Color(50, 50, 50, 255));

			for (int i = 0; i < elements.size(); i++)
			{
				if (Input->Hovering(this->position.x, this->position.y + 17 + i * 16, 170, 16))
				{
					DrawRectGradientVertical(this->position.x, this->position.y + 14 + i * 16, 170, 16, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
				}

				if (Input->Clicked(this->position.x, this->position.y + 17 + i * 16, 170, 16))
				{
					if (*this->Var >= 0 && *this->Var < elements.size())
					{
						*this->Var = i;
						*this->IsOpen = false;
					}
				}
				DrawString(Verdana, this->position.x + 10, this->position.y + 21 + i * 16, Color(255, 255, 255, 255), FONT_LEFT, elements[i].c_str());
			}
		}
		DrawString(Verdana, this->position.x, this->position.y - 17, Color(255, 255, 255, 255), FONT_LEFT, name.c_str());
	}
}


MCombo::MCombo(MGroup* parent, std::string name, int* variable, Vector2D pos, std::vector<std::string> elements, bool* fuck)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->position = pos;
	this->elements = elements;
	this->IsOpen = fuck;
}


void MCombo::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y, 170, 20))
		if (*this->IsOpen == false)
			*this->IsOpen = true;
		else
			*this->IsOpen = false;

	if (elements.size() > 0)
	{
		DrawRectGradientVertical(this->position.x, this->position.y - 8, 170, 20, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 9, 172, 22, Color::Black());
		DrawString(Verdana, this->position.x + 11, this->position.y + 2, Color(255, 255, 255, 255), FONT_LEFT, this->elements[*this->Var].c_str());

		if (*this->IsOpen)
		{
			DrawRectGradientVertical(this->position.x, this->position.y + 14, 170, elements.size() * 16, Color(70, 70, 70, 255), Color(50, 50, 50, 255));

			for (int i = 0; i < elements.size(); i++)
			{
				if (Input->Hovering(this->position.x, this->position.y + 17 + i * 16, 170, 16))
				{
					DrawRectGradientVertical(this->position.x, this->position.y + 14 + i * 16, 170, 16, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
				}

				if (Input->Clicked(this->position.x, this->position.y + 17 + i * 16, 170, 16))
				{
					if (*this->Var >= 0 && *this->Var < elements.size())
					{
						*this->Var = i;
						*this->IsOpen = false;
					}
				}
				DrawString(Verdana, this->position.x + 10, this->position.y + 21 + i * 16, Color(255, 255, 255, 255), FONT_LEFT, elements[i].c_str());
			}
		}
		DrawString(Verdana, this->position.x, this->position.y - 17, Color(255, 255, 255, 255), FONT_LEFT, name.c_str());
	}
}


MComboNoParent::MComboNoParent(std::string name, int* variable, Vector2D pos, std::vector<std::string> elements, bool* fuck)
{
	this->name = name;
	this->Var = variable;
	this->position = pos;
	this->elements = elements;
	this->IsOpen = fuck;
}


void MComboNoParent::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y, 170, 20))
		if (*this->IsOpen == false)
			*this->IsOpen = true;
		else
			*this->IsOpen = false;

	if (elements.size() > 0)
	{
		DrawRectGradientVertical(this->position.x, this->position.y - 8, 170, 20, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
		DrawOutlinedRect(this->position.x - 1, this->position.y - 9, 172, 22, Color::Black());
		DrawString(Verdana, this->position.x + 11, this->position.y + 2, Color(255, 255, 255, 255), FONT_LEFT, this->elements[*this->Var].c_str());

		if (*this->IsOpen)
		{
			DrawRectGradientVertical(this->position.x, this->position.y + 14, 170, elements.size() * 16, Color(70, 70, 70, 255), Color(50, 50, 50, 255));

			for (int i = 0; i < elements.size(); i++)
			{
				if (Input->Hovering(this->position.x, this->position.y + 17 + i * 16, 170, 16))
				{
					DrawRectGradientVertical(this->position.x, this->position.y + 14 + i * 16, 170, 16, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
				}

				if (Input->Clicked(this->position.x, this->position.y + 17 + i * 16, 170, 16))
				{
					if (*this->Var >= 0 && *this->Var < elements.size())
					{
						*this->Var = i;
						*this->IsOpen = false;
					}
				}
				DrawString(Verdana, this->position.x + 10, this->position.y + 21 + i * 16, Color(255, 255, 255, 255), FONT_LEFT, elements[i].c_str());
			}
		}
		DrawString(Verdana, this->position.x, this->position.y - 17, Color(255, 255, 255, 255), FONT_LEFT, name.c_str());
	}
}

MSelector::MSelector(MGroup* parent, std::string name, int* variable, Vector2D pos, std::vector<std::string> elements)
{
	this->parent = parent;
	this->name = name;
	this->caption = caption;
	this->Var = variable;
	this->position = pos;
	this->elements = elements;
}

void MSelector::Draw()
{
	if (Input->Clicked(this->position.x, this->position.y + 18, 10, 10))
	{
		if (*this->Var > 0) *this->Var -= 1;
	}

	if (Input->Clicked(this->position.x + 150, this->position.y + 18, 10, 10))
	{
		if (*this->Var >= 0 && *this->Var < this->elements.size() - 1) *this->Var += 1;
	}

	DrawString(Verdana, this->position.x, this->position.y, Color(201, 201, 201, 255), FONT_LEFT, "%s", this->name.c_str());
	DrawString(Verdana, this->position.x, this->position.y + 18, Color(201, 201, 201, 255), FONT_LEFT, "<");

	DrawRectGradientVertical(this->position.x + 15, this->position.y + 12, 125, 12, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
	DrawOutlinedRect(this->position.x + 14, this->position.y + 11, 127, 14, Color::Black());
	DrawString(Verdana, this->position.x + 15 + (125 / 2), this->position.y + 18, Color(201, 201, 201, 255), FONT_CENTER, "%s", this->elements[*this->Var].c_str());
	DrawString(Verdana, this->position.x + 150, this->position.y + 18, Color(201, 201, 201, 255), FONT_LEFT, ">");
}

MSlider::MSlider(MGroup* parent, std::string name, int* variable, int min, int max, int width, Vector2D pos)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->min = min;
	this->max = max;
	this->width = width;
	this->position = pos;
}

void MSlider::Draw()
{
	DrawString(Verdana, this->position.x, this->position.y + 5, Color(200, 200, 200, 255), FONT_LEFT, "%s", this->name.c_str());
	DrawRectGradientVertical(this->position.x, this->position.y + 18, this->width + 1, 9, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
	DrawRectGradientVertical(this->position.x, this->position.y + 19, (*this->Var * this->width / this->max), 8, Color(37, 195, 219,255), Color(37, 195, 219,255));
	DrawOutlinedRect(this->position.x - 1, this->position.y + 17, this->width + 2, 10, Color::Black());
	DrawString(Verdana, this->position.x + this->width - 20, this->position.y + 5, Color(200, 200, 200, 150), FONT_LEFT, "%i", *this->Var);

	if (Input->Hovering(this->position.x, this->position.y + 19, this->width + 2, 13))
	{
		DrawString(Verdana, this->parent->position.x + 130, this->parent->position.y + this->parent->size.y - 20, Color(255, 255, 255), FONT_LEFT, "%s", this->caption.c_str());
		if (GetAsyncKeyState(0x01) & 0x8000)
		{
			POINT Cur; GetCursorPos(&Cur);
			auto diff = (Cur.x - (this->position.x + 1));
			float perc = (diff / this->width);
			float range = max - min;
			range = range * perc;
			float Value = Clamp(roundf(min + range), min, max);
			*Var = Value;
		}
	}
}

MSliderFloat::MSliderFloat(MGroup* parent, std::string name, float* variable, float min, float max, int width, Vector2D pos)
{
	this->parent = parent;
	this->name = name;
	this->Var = variable;
	this->min = min;
	this->max = max;
	this->width = width;
	this->position = pos;
}

void MSliderFloat::Draw()
{
	DrawString(Verdana, this->position.x, this->position.y + 5, Color(200, 200, 200, 255), FONT_LEFT, "%s", this->name.c_str());
	DrawRectGradientVertical(this->position.x, this->position.y + 18, this->width + 1, 9, Color(70, 70, 70, 255), Color(50, 50, 50, 255));
	DrawRectGradientVertical(this->position.x, this->position.y + 19, (*this->Var * this->width / this->max), 8, Color(37, 195, 219,255), Color(37, 195, 219,255));
	DrawOutlinedRect(this->position.x - 1, this->position.y + 17, this->width + 2, 10, Color::Black());
	DrawString(Verdana, this->position.x + this->width - 20, this->position.y + 5, Color(200, 200, 200, 150), FONT_LEFT, "%.1f", *this->Var);

	if (Input->Hovering(this->position.x, this->position.y + 19, this->width + 2, 13))
	{
		if (GetAsyncKeyState(0x01) & 0x8000)
		{
			POINT Cur; GetCursorPos(&Cur);
			float diff = (Cur.x - (this->position.x + 1));
			float perc = (diff / this->width);
			float range = max - min;
			range = range * perc;
			float Value = Clamp(min + range, min, max);
			*Var = Value;
		}
	}
}