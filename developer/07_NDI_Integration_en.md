# NDI Media Integration: Follow the Texture Change Hooks Used by Screens and Projectors

[Back to Contents](README_en.md) · [Project Media Example](examples/ProjectMediaReceiver.h)

## 1. Correct Entry Point

The exposed core type is `ASuperMediaBase : ASuperBaseActor`, declared in SuperMediaBase.h as an Abstract class. Existing screen and projector objects both override `OnActiveTextureChanged()`, obtain `GetActiveTexture()` there, and update material texture references. Project media applications use the same entry point.

Do not create your own NDI receiver, include the non-exposed SuperNdi module, or treat DMX's SuperDMXChanged as a video-frame event. ASuperMediaBase does not inherit ASuperDmxActorBase. If both DMX and NDI are needed, use two control objects with separate responsibilities and apply their data to the same application target; do not use multiple inheritance from two Actor base classes.

## 2. Configuration Fields

| Field | Default / purpose | Blueprint boundary |
| --- | --- | --- |
| SourceMode | Texture; enum NDI/Texture/Director | EditAnywhere, not declared BlueprintReadWrite |
| InputName | NAME_None; logical name of a configured input | Displayed as NDIInputSelection in the editor; dropdown supplied by GetInputNameOptions |
| StaticTexture | nullptr; UTexture for Texture mode | Editor configuration |
| DirectorCamera | nullptr; source for Director mode | Editor configuration; project NDI reuse does not require a dependency on the concrete camera class |
| NDITexture | UTexture2D created at runtime | Transient, VisibleAnywhere; not an arbitrarily writable Blueprint texture variable |

If InputName is empty, the current binding logic attempts to select the first configured input. Explicitly select a name in production projects to avoid depending on configuration order. GetInputNameOptions has UFUNCTION but is not marked BlueprintCallable/Pure; it provides Details panel options, not evidence for creating a callable Blueprint node.

Changing SourceMode/InputName in the editor triggers the base class's configuration handling. Directly assigning fields in runtime C++ is not equivalent to triggering editor property notifications. These examples configure the source in advance in the editor and do not invent a generic runtime source-switching node.

## 3. GetActiveTexture

`UTexture* GetActiveTexture() const` is a public C++ function with no UFUNCTION.

NDI returns NDITexture; Texture returns StaticTexture; Director returns the associated capture texture, or null without a camera. The return type is always UTexture. Consumers must not unconditionally cast it to UTexture2D, since other modes may return a RenderTarget.

It may be null before the first frame. A valid texture object does not mean the sender is still online; stream loss may retain the last image. The texture does not prove that “a packet just arrived.” Define an explicit fallback-texture or blank-image policy for null, and do not treat a retained frame as live status.

## 4. Actual OnActiveTextureChanged Triggers

This is a protected C++ virtual method with an empty base implementation, **not a native BlueprintImplementableEvent or a bindable delegate**.

| Path | Behavior |
| --- | --- |
| BeginPlay, OnConstruction | The base class binds according to configuration, then calls the hook |
| Editor property change | Pushes the current active texture |
| Component registration/initialization compensation | The base class schedules a deferred initial push |
| The first NDI frame creates a texture | Notifies the texture reference after upload |
| An NDI size change recreates the texture | Notifies the new texture reference |
| Ordinary NDI frame of the same size | Updates existing texture contents; **does not call this hook every frame** |

The header comment mentions “a new frame arrives,” but the current implementation notifies from the frame path only on creation/recreation. When a material holds the same UTexture, GPU content updates display new images without calling SetTextureParameter again for every video frame.

The hook may run before the derived object's material is ready, so handling must be repeatable and accept null. Existing screens/projectors prepare their MIDs during construction and initialization, and the base class provides a deferred compensation call. The project example ensures its own MID is valid inside the hook before applying the texture, avoiding reliance on a single notification.

## 5. Reception Methods in the Header

| Method | Confirmed behavior | Project usage principle |
| --- | --- | --- |
| BindToSubsystem() | Binds by InputName, removes the old binding when the name changes, then deduplicates the subscription | Managed by the base lifecycle; do not call manually every frame |
| HandleNDIFrame(BGRA,Width,Height) | Handles NDI mode only; validates dimensions >0 and data length=Width×Height×4, then creates/uploads the texture | Base reception handler, not a public video delegate for users to bind |
| EnsureTexture(Width,Height) | Reuses matching dimensions; creates a Transient texture for different dimensions, returning whether creation succeeded | Do not preemptively create NDITexture or manually change its dimensions |
| UpdateTextureGPU(SrcBGRA,Width,Height) | Passes data to the rendering upload path | Part of the base-managed upload flow; these texture consumers do not call it directly |
| GetInputNameOptions() | Returns configured input names | Supplies the source-selection dropdown; not declared Blueprint Pure |
| EndPlay and other lifecycle methods | Unsubscribe and perform normal resource management | Derived overrides must preserve parent calls; the example needs no overrides |

The presence of these functions in an exposed header does not mean they should all become part of the project's own reception flow. NDI consumers can follow the existing screen's hook pattern without entering internal protocol/rendering management.

## 6. Complete Project Receiver and Material Example

Files: [ProjectMediaReceiver.h](examples/ProjectMediaReceiver.h), [ProjectMediaReceiver.cpp](examples/ProjectMediaReceiver.cpp). It inherits ASuperMediaBase and overrides only OnActiveTextureChanged. TargetActor, BaseMaterial, MaterialIndex, TextureParameter, and FallbackTexture are project additions, not base-class properties.

1. Verify that the SuperMediaBase.h include chain and module linking are available in the official delivery SDK, then compile the example.
2. Create your own material, add a Texture Sample Parameter2D named ProjectMedia, and connect it to the desired material output. For a model-display example, start with a simple Unlit output to eliminate lighting interference.
3. Place ProjectMediaReceiver and select SourceMode=NDI and an explicit InputName. TargetActor references your StaticMeshActor; select your BaseMaterial, a valid MaterialIndex, and a project fallback image as FallbackTexture.
4. The hook ensures its MID exists, assigns it to the target slot, and writes the active or fallback texture. It recreates its own MID after object duplication or a BaseMaterial change.
5. First confirm the material/UVs with a static image in Texture mode, then switch to NDI to verify moving video. Changing the sender's resolution should correctly replace the texture reference.

FallbackTexture in this example is used only when GetActiveTexture has no valid object. Losing the stream while the texture still exists does not automatically switch to the fallback. Fallback behavior is project logic, not product online-status detection.

## 7. Using Media from Blueprint

The native GetActiveTexture and OnActiveTextureChanged do not have DMX-style Blueprint declarations. Do not simply search as in a DMX Blueprint and assume same-named nodes exist. The supplied ProjectMediaReceiver adds:

| New project interface | Function |
| --- | --- |
| GetProjectTexture | BlueprintPure; returns the active texture or project FallbackTexture |
| ProjectTextureChanged | BlueprintImplementableEvent; forwards texture reference changes to the derived Blueprint in the running world |

Compile the two example files before these project nodes exist. ProjectTextureChanged is an event implemented by the derived Blueprint, not an external Dispatcher; declare a project Dispatcher if cross-object broadcasting is needed.

This example event notifies only in a game world after the Actor has begun play. The C++ hook handles editor material preview; no guarantee is made that this project Blueprint event executes in the non-running editor. Consumers such as UI should read GetProjectTexture once during initialization, then receive reference updates through project forwarding, so UI created after the first notification does not miss the initial texture.

## 8. UMG and Render Target Applications

UMG: create a UI-domain material with a ProjectMedia texture parameter, create your UI MID, and use it as the Image Brush material. When the UI is ready, obtain UTexture from the receiver and write the parameter; write again when its reference changes. Ordinary frame updates are reflected in the held texture's contents, without repeatedly creating Widgets/MIDs. Do not unconditionally connect a UTexture result to a node that accepts only UTexture2D; using a material preserves texture-type generality.

Render Target: if the target system requires a separate RenderTarget, the project's rendering flow can draw a material sampling this texture into the target. However, one draw produces only that image; ordinary NDI frames do not trigger OnActiveTextureChanged every frame. This hook is suitable only for rebinding source references, not as a continuous-copy clock. Design and validate a separate project rendering-update mechanism. This documentation does not describe a single copy as a continuous video stream.

Multiple materials consuming one texture: update multiple MIDs in a single hook, as the existing projector example does. Each consumer retains its own MID and parameter configuration, avoiding a separate NDI input for every object.

## 9. Media Acceptance Checks

Record source configuration, null/fallback behavior before the first frame, initial texture reference establishment, ongoing image changes on ordinary frames, reference recreation after resolution changes, Texture/NDI mode switching, MID independence after duplication, invalid-target handling, and cleanup at the end of play. When disconnecting the sender, record actual behavior rather than promising an automatic black screen.

Compilation against a customer SDK and real-stream testing have not yet been completed. If exposed core headers reference undelivered dependencies, fix delivery-package completeness first; do not compensate for this documentation by exposing other modules' source code to users.
