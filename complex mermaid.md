```mermaid

classDiagram
    class Graphics {
        +BeginFrame(float,float,float)
        +EndFrame()
        +DrawIndexed(UINT)
        +SetProjection(FXMMATRIX)
        +SetCamera(FXMMATRIX)
        +GetCamera() : XMMATRIX
        +GetProjection() : XMMATRIX
    }

    class Bindable {
        <<abstract>>
        +Bind(Graphics&)
        #GetContext(Graphics&)
        #GetDevice(Graphics&)
    }

    class Drawable {
        +GetTransformXM() : XMMATRIX
        +Update(float)
        +Draw(Graphics&)
    }

    class DrawableBase {
        +IsStaticInitialized() : bool
        +AddStaticBind(unique_ptr<Bindable>)
        +AddStaticIndexBuffer(unique_ptr<IndexBuffer>)
        +SetIndexFromStatic()
        -staticBinds : vector<unique_ptr<Bindable>>
    }

    class Box {
        +Box(Graphics&, rng,...)
        +Update(float)
        +GetTransformXM() : XMMATRIX
    }

    class SkinnedBox {
        +SkinnedBox(Graphics&, rng,...)
        +Update(float)
        +GetTransformXM() : XMMATRIX
    }

    class Cube {
        +Make<V>()
        +MakeSkinned<V>()
    }

    class VertexBuffer {
        +VertexBuffer(Graphics&, const vector<V>&)
        +Bind(Graphics&)
    }

    class IndexBuffer {
        +IndexBuffer(Graphics&, const vector<UINT>&)
        +GetCount() : UINT
        +Bind(Graphics&)
    }

    class VertexShader {
        +VertexShader(Graphics&, wstring)
        +Bind(Graphics&)
        +GetBytecode() : ID3DBlob*
    }

    class PixelShader {
        +PixelShader(Graphics&, wstring)
        +Bind(Graphics&)
    }

    class InputLayout {
        +InputLayout(Graphics&, vector<D3D11_INPUT_ELEMENT_DESC>, ID3DBlob*)
        +Bind(Graphics&)
    }

    class Topology {
        +Topology(Graphics&, D3D11_PRIMITIVE_TOPOLOGY)
        +Bind(Graphics&)
    }

    class TransformCbuf {
        +TransformCbuf(Graphics&, const Drawable&)
        +Bind(Graphics&)
        -parent : const Drawable&
        -static pVcbuf : unique_ptr<VertexConstantBuffer<XMMATRIX>>
    }

    class VertexConstantBuffer {
        +Update(Graphics&, const C&)
        +Bind(Graphics&)
    }

    class PixelConstantBuffer {
        +Update(Graphics&, const C&)
        +Bind(Graphics&)
    }

    class Texture {
        +Texture(Graphics&, Surface)
        +Bind(Graphics&)
    }

    class Sampler {
        +Sampler(Graphics&)
        +Bind(Graphics&)
    }

    class Surface {
        +static FromFile(string) : Surface
    }

    Bindable <|-- VertexBuffer
    Bindable <|-- IndexBuffer
    Bindable <|-- VertexShader
    Bindable <|-- PixelShader
    Bindable <|-- InputLayout
    Bindable <|-- Topology
    Bindable <|-- TransformCbuf
    Bindable <|-- VertexConstantBuffer
    Bindable <|-- PixelConstantBuffer
    Bindable <|-- Texture
    Bindable <|-- Sampler

    Drawable <|-- DrawableBase
    DrawableBase <|-- Box
    DrawableBase <|-- SkinnedBox

    Cube --> VertexBuffer : "provides vertices"
    SkinnedBox --> Texture : "uses"
    SkinnedBox --> Sampler : "uses"
    SkinnedBox --> TransformCbuf : "adds instance bind"
    Drawable o-- Bindable : "has binds"
    Graphics o-- Bindable : "friend / access to device/context"
    TransformCbuf --> VertexConstantBuffer : "wraps / updates"
    Surface --> Texture : "used to create"