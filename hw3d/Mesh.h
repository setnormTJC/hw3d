#pragma once

#include"BindableCommon.h"
#include "ConditionalNoexcept.h"
#include"DrawableBase.h"
#include"Vertex.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>


#include<memory>

class Mesh : public DrawableBase<Mesh>
{
public: 
	Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);

	/**/
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private: 
	mutable DirectX::XMFLOAT4X4 transform; 
};




class Node
{
	friend class Model; 

public: 
	Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noxnd;
	/*Node's Draw calls Mesh::Draw*/
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
private: 
	/*Only called by Model (a friend of this class)*/
	void AddChild(std::unique_ptr<Node> pChild) noxnd;
private: 
	std::vector<std::unique_ptr<Node>> childPtrs; 
	std::vector<Mesh*> meshPtrs; 
	DirectX::XMFLOAT4X4 transform;
};

class Model
{
public: 
	Model(Graphics& gfx, const std::string filename);
	/*Model's Draw calls Node::Draw*/
	void Draw(Graphics& gfx, DirectX::FXMMATRIX transform) const;

private: 
	/*WORKHORSE that binds:
	1) vertex buffer
	2) index buffer
	3) vertex shader
	4) pixel shader
	5) input layout
	6)*/
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);

	std::unique_ptr<Node> ParseNode(const aiNode& node);

private: 
	std::unique_ptr<Node> pRoot; 
	std::vector<std::unique_ptr<Mesh>> meshPtrs; //same member var as in Node - but SMART this time
												//(see comment in `ParseNode` for more details on why)
};