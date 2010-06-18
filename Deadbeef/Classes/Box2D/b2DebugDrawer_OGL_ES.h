/*
 *  b2DebugDrawer_OGL_ES.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/13/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#pragma once
#include "Box2D.h"
#include "Shader.h"
#include "Matrix.h"

class b2DebugDrawer_OGL_ES : public b2DebugDraw 
{
public:
	b2DebugDrawer_OGL_ES();
	
	void SetProjection(const Deadbeef::Matrix &projection) { this->projection = projection; }
private:
	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	
	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	
	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	
	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	
	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	
	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform& xf);
	
	int vertexBufferHandle;

	//Attribute Handles
	int posAttributeLoc;
	//Uniform Handles
	int wvpUniformLoc;
	int colorUniformLoc;
	
	Deadbeef::Matrix projection;
	Deadbeef::ShaderRef shader;
};
