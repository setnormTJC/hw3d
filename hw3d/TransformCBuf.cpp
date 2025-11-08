#include "TransformCBuf.h"

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent)
	:
	vcbuf(gfx),
	parent(parent)
{
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{

    vcbuf.Update(gfx,
        DirectX::XMMatrixTranspose(
            parent.GetTransformXM() * gfx.GetProjection()
        )
    );
    //vcbuf.Update(gfx,
    //    XMMatrixTranspose(
    //        parent.GetTransformXM() * XMLoadFloat4x4(&gfx.GetProjection()) //differs from CHili approach!
    //    ));
    //
    vcbuf.Bind(gfx);
}
