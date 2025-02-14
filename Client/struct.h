#pragma once

struct Vec2 {
	float x;
	float y;

public:
	float Length() {
		return sqrt(x * x + y * y);
	}

	bool IsZero() {
		if (x == 0.f && y == 0.f) return true;
		else return false;
	}
	Vec2& Normalize() {
		float fLen = Length();
		
		if (fLen == 0.f) {
			return *this;
		}

		x /= fLen;
		y /= fLen;

		return *this;
	}


	Vec2& operator = (POINT _pt) {
		x = (float)_pt.x;
		y = (float)_pt.y;
	}

	Vec2 operator + (Vec2 _vOther) {
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	void operator += (Vec2 _vOther) {
		x += _vOther.x;
		y += _vOther.y;
	}

	Vec2 operator - (Vec2 _vOther) {
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	Vec2 operator * (Vec2 _vOther) {
		return Vec2(x * _vOther.x, y * _vOther.y);
	}

	Vec2 operator * (int _ivalue) {
		return Vec2(x * (float)_ivalue, y * (float)_ivalue);
	}

	Vec2 operator * (float _fvalue) {
		return Vec2(x * _fvalue, y * _fvalue);
	}

	Vec2 operator / (Vec2 _vOther) {
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return Vec2(x / _vOther.x, y / _vOther.y);
	}
	Vec2 operator / (float _fValue) {
		assert(!(_fValue == 0.f));
		return Vec2(x / _fValue, y / _fValue);
	}

public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(int _x, int _y)
		: x((float)_x)
		, y((float)_y)
	{}

	Vec2(const POINT& _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}
};