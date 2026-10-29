## Editor Engine Class Diagram

```mermaid
classDiagram

namespace EngineApp {
	class EditorEngine
	class FEngineTimer
}

namespace Rendering {
	class URenderer
}

namespace Editor {
	class FEditorUiManager
	class FEditorViewport
}

namespace Core.Math {
	class FCamera
}

namespace Object {
	class USceneComponent
}

namespace RenderInterface {
	class FPrimitiveRenderData
}

EditorEngine *-- FEngineTimer
EditorEngine *-- URenderer
EditorEngine *-- FEditorUiManager
EditorEngine *-- FEditorViewport
EditorEngine *-- FCamera
EditorEngine *-- USceneComponent

USceneComponent ..> FPrimitiveRenderData
URenderer ..> FPrimitiveRenderData

```