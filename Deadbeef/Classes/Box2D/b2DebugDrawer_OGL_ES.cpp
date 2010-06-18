/*
 *  b2DebugDrawer_OGL_ES.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/13/10.
 *  Copyright 2010  . All rights reserved.
 *
 */
#include "VertexFormats.h"
#include "b2DebugDrawer_OGL_ES.h"
#include "Vect2.h"
#include <stdio.h>
#include "ShaderManager.h"

b2DebugDrawer_OGL_ES::b2DebugDrawer_OGL_ES()
{
	shader = Deadbeef::ShaderManager::GetInstance().ShaderNamed("Box2DDebug");
	posAttributeLoc = glGetAttribLocation(shader->program, "position");
	
	//Setup uniform handles
	wvpUniformLoc = glGetUniformLocation(shader->program, "wvp");
	colorUniformLoc = glGetUniformLocation(shader->program, "color");
}

/// Draw a closed polygon provided in CCW order.
void b2DebugDrawer_OGL_ES::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glUseProgram(shader->program);

	//Enable attirbute slots
	glEnableVertexAttribArray(posAttributeLoc);
	glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(b2Vec2), vertices);
	
	//Set Uniform shader
	glUniformMatrix4fv(wvpUniformLoc, 1, false, projection.m);
	glUniform3fv(colorUniformLoc, 1, (const GLfloat*)&color);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
	glLineWidth(3.0f);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(posAttributeLoc);
}

/// Draw a solid closed polygon provided in CCW order.
void b2DebugDrawer_OGL_ES::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glUseProgram(shader->program);
	
	//Enable attirbute slots
	glEnableVertexAttribArray(posAttributeLoc);
	glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(b2Vec2), vertices);
	
	//Set Uniform shader
	glUniformMatrix4fv(wvpUniformLoc, 1, false, projection.m);
	glUniform3fv(colorUniformLoc, 1, (const GLfloat*)&color);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	glDisable(GL_BLEND);
	
	glDisableVertexAttribArray(posAttributeLoc);
}

/// Draw a circle.
void b2DebugDrawer_OGL_ES::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	int32 count = 36;
	Deadbeef::Vect2 verts[count];
	float theta = (360.0f / (float)count) *(M_PI / 180.0f);// degrees to radians
	float dist = radius;
	
	for (int i = 0; i < count; ++i)
	{
		Deadbeef::Vect2 temp(cos((float)i * (theta)), sin((float)i * (theta)));
		temp = temp * dist;
		verts[i] = temp;
	}
	
	glUseProgram(shader->program);
	
	//Enable attirbute slots
	glEnableVertexAttribArray(posAttributeLoc);
	glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(b2Vec2), verts);
	
	//Set Uniform shader
	glUniformMatrix4fv(wvpUniformLoc, 1, false, projection.m);
	glUniform3fv(colorUniformLoc, 1, (const GLfloat*)&color);
	
	glEnable(GL_BLEND);
	glLineWidth(3.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
	glDrawArrays(GL_LINE_LOOP, 0, count);
	glDisable(GL_BLEND);
	
	glDisableVertexAttribArray(posAttributeLoc);
	
}

/// Draw a solid circle.
void b2DebugDrawer_OGL_ES::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	int32 count = 36;
	b2Vec2 verts[count];
	float theta = (360.0f / (float)count) *(M_PI / 180.0f);// degrees to radians
	float dist = radius;
	
	for (int i = 0; i < count; ++i)
	{
		b2Vec2 temp(cos((float)i * (theta)), sin((float)i * (theta)));
		temp = dist * temp;
		verts[i] = center + temp;
	}
	
	glUseProgram(shader->program);
	
	//Enable attirbute slots
	glEnableVertexAttribArray(posAttributeLoc);
	glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(b2Vec2), verts);
	
	//Set Uniform shader
	glUniformMatrix4fv(wvpUniformLoc, 1, false, projection.m);
	glUniform3fv(colorUniformLoc, 1, (const GLfloat*)&color);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
	glDrawArrays(GL_TRIANGLE_FAN, 0, count);
	glDisable(GL_BLEND);
	
	glDisableVertexAttribArray(posAttributeLoc);
}

/// Draw a line segment.
void b2DebugDrawer_OGL_ES::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glUseProgram(shader->program);
	
	b2Vec2 vecs[2] = {p1, p2};
	
	//Enable attirbute slots
	glEnableVertexAttribArray(posAttributeLoc);
	glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(b2Vec2), vecs);
	
	//Set Uniform shader
	glUniformMatrix4fv(wvpUniformLoc, 1, false, projection.m);
	glUniform3fv(colorUniformLoc, 1, (const GLfloat*)&color);
	
	glEnable(GL_BLEND);
	glLineWidth(3.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
	glDrawArrays(GL_LINES, 0, 2);
	glDisable(GL_BLEND);
	
	glDisableVertexAttribArray(posAttributeLoc);
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void b2DebugDrawer_OGL_ES::DrawTransform(const b2Transform& xf)
{
	debug_log("Draw Transform\n");
}