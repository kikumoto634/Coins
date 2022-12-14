#pragma once

#include <DirectXMath.h>
#include "Vector3.h"
#include "Matrix4.h"
#include "Input.h"

/// <summary>
/// カメラ
/// </summary>
class Camera
{
private:
	const float distance = 200.f;	//カメラの距離
	const float upRot = 90.f;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input);
	void CameraPosInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();


	//getter
	const DirectX::XMMATRIX& GetMatProjection()	{return matProjection;}
	const DirectX::XMMATRIX& GetMatView()	{return matView;}
	const DirectX::XMMATRIX& GetViewProjectionMatrix()	{return matViewProjection;}

	const Vector3& GetUp() {return up; }

	/// <summary>
	/// カメラ位置移動
	/// </summary>
	/// <param name="pos"></param>
	void SetMove(Vector3 move)	{this->move = move;}
	void SetVelocity(float velocity)	{this->velocity = velocity;}

	/// <summary>
	/// 注視点座標移動
	/// </summary>
	const Vector3& GetTarget() {return target; }
	void SetTarget(Vector3 target)	{this->target = target;};
	
	/// <summary>
	/// 視点座標
	/// </summary>
	const Vector3& GetEye() {return eye; }
	void SetEye(Vector3 eye)	{this->eye = eye;};

	/// <summary>
	/// ニアクリップ移動
	/// </summary>
	const float& GetNearZ() {return nearZ; }
	void SetNearZ(float nearZ)	{this->nearZ = nearZ;};

	/// <summary>
	/// ファークリップ移動
	/// </summary>
	const float& GetFarZ() {return farZ; }
	void SetFarZ(float farZ)	{this->farZ = farZ;};

	//座標
	const Vector3& GetPosition()	{return position;}

private:

	//透視投影
	DirectX::XMMATRIX matProjection;	//プロジェクション行列
	//ビュー変換行列
	DirectX::XMMATRIX matView;		//ビュー行列
	Vector3 eye;			//視点座標
	Vector3 target;		//注視点座標
	Vector3 up;			//上方向ベクトル

	float nearZ;
	float farZ;

	DirectX::XMMATRIX matViewProjection;

	Vector3 position = {0,0,0};
	Vector3 move;
	float velocity;

	Input* input = nullptr;
};

