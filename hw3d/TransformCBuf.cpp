#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
    if (!pVcbuf)
    {
        //pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx); 
        pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
    }
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
    const auto model = parent.GetTransformXM(); 

    const Transforms tf =
    {
        DirectX::XMMatrixTranspose(model), 
        //transpose is here because GPU expects row major but we store in col. major
        //(or the opposite of that - can't remember)
        DirectX::XMMatrixTranspose(
            model *
            gfx.GetCamera() *
            gfx.GetProjection()
        )
    };

    pVcbuf->Update(gfx, tf);
    pVcbuf->Bind(gfx);
}

//define the static member var: 
//std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pVcbuf;
std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
