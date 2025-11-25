#pragma once

#include"BindableCommon.h"
#include "ConditionalNoexcept.h"
#include"DrawableBase.h"
#include"Vertex.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<memory>
#include<optional>


class ModelException : public ChiliException
{
public:
	ModelException(int line, const char* file, std::string note) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string note;
};




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
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) noxnd;
	/*Node's Draw calls Mesh::Draw*/
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
	int GetId() const noexcept; 
	void ShowTree(Node*& pSelectedNode) const noexcept;
private: 
	/*Only called by Model (a friend of this class)*/
	void AddChild(std::unique_ptr<Node> pChild) noxnd;
private: 
	std::string name; //ex: arm or head of some model (stored in .obj file) 
	int id; 
	std::vector<std::unique_ptr<Node>> childPtrs; 
	std::vector<Mesh*> meshPtrs; 
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;
};



class Model
{
public: 
	Model(Graphics& gfx, const std::string filename);
	/*Model's Draw calls Node::Draw*/
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(const char* windowName = nullptr) noexcept;
	~Model() noexcept; 
private: 
	/*WORKHORSE that binds:
	1) vertex buffer
	2) index buffer
	3) vertex shader
	4) pixel shader
	5) input layout
	6)*/
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);

	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node);

private: 
	std::unique_ptr<Node> pRoot; 
	std::vector<std::unique_ptr<Mesh>> meshPtrs; //same member var as in Node - but SMART this time
												//(see comment in `ParseNode` for more details on why)

	std::unique_ptr<class ModelWindow> pWindow; //yet another one

};