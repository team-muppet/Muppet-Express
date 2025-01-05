<script>
    let { Id = $bindable(0), Name = $bindable(0), Image = $bindable(0), pokemonList = $bindable([]) } = $props();

    let isEditing = $state(false);
    let editedName = $state(Name);
    let editedImage = $state(Image);

    import Icon from "@iconify/svelte";
    import { FileUpload } from "@skeletonlabs/skeleton-svelte";
    import { Modal } from "@skeletonlabs/skeleton-svelte";
    import { getContext } from "svelte";
    const toast = getContext("toast");

    async function saveEdit() {
        try {
            const updatedPokemon = { Id: Id, Name: editedName, Image: editedImage };

            const response = await fetch(`/api/pokemon/${Id}`, {
                method: "PUT",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(updatedPokemon),
            });

            if (!response.ok) {
                throw new Error(`Failed to edit Pokémon: ${response.statusText}`);
            }

            // Update the Pokémon in the list
            const pokemonIndex = pokemonList.findIndex((p) => p.Id === Id);
            if (pokemonIndex !== -1) {
                pokemonList[pokemonIndex] = updatedPokemon;
            }

            Name = editedName;
            Image = editedImage;

            toast.create({
                title: "Success",
                description: "Pokémon updated successfully!",
                type: "success",
            });

            isEditing = false;
        } catch (error) {
            console.error(error);
            toast.create({
                title: "Error",
                description: "An error occurred while editing the Pokémon.",
                type: "error",
            });
        }
    }

    async function deletePokemon() {
        try {
            const response = await fetch(`/api/pokemon/${Id}`, {
                method: "DELETE",
            });

            if (!response.ok) {
                throw new Error(`Failed to delete Pokémon: ${response.statusText}`);
            }

            // Remove the Pokémon from the list
            pokemonList = pokemonList.filter((p) => p.Id !== Id);

            toast.create({
                title: "Success",
                description: "Pokémon deleted successfully!",
                type: "success",
            });
        } catch (error) {
            console.error(error);
            toast.create({
                title: "Error",
                description: "An error occurred while deleting the Pokémon.",
                type: "error",
            });
        }
    }

    async function generatePreview(event) {
        const reader = new FileReader();

        const files = event.acceptedFiles || [];

        if (files.length > 0) {
            const selectedFile = files[0];
            editedImage = await new Promise((resolve, reject) => {
                reader.onload = () => resolve(reader.result);
                reader.onerror = () => reject(new Error("Failed to read file"));
                reader.readAsDataURL(selectedFile);
            });
        } else {
            editedImage = null;
        }
    }

    function modalClose() {
        isEditing = false;
    }

    function modalOpen() {
        isEditing = true;
    }
</script>

<div class="card preset-filled-tertiary-50-950 size-72 p-4 relative overflow-hidden group">
    <!-- Normal Display -->
    <img class="absolute inset-0 w-full h-full object-cover" alt="A Pokémon" src={Image} />
    <div class="absolute bottom-1 left-2 bg-white/30 backdrop-blur-md rounded-md px-1">
        <span class="text-3xl">{Name}</span>
    </div>
    <div class="absolute bottom-1 right-1">
        <span class="badge preset-outlined-secondary-500 preset-tonal-secondary">
            #{Id}
        </span>
    </div>
    <!-- Edit and Delete Buttons on Hover -->
    <div class="absolute inset-0 flex justify-center items-center opacity-0 group-hover:opacity-100 bg-black/50 transition-opacity">
        <Modal bind:open={isEditing} contentBase="card bg-surface-100-900 p-6 space-y-4 shadow-xl max-w-screen-sm w-full" backdropClasses="backdrop-blur-sm">
            {#snippet trigger()}
                <div class="btn preset-filled mr-2">Edit</div>
            {/snippet}
            {#snippet content()}
                <header class="flex justify-between">
                    <h2 class="h2">Add Pokemon</h2>
                </header>
                <article class="flex flex-col gap-4">
                    <div class="mx-auto">
                        <div class="card preset-filled-tertiary-50-950 size-72 p-4 relative overflow-hidden">
                            <!-- Normal Display -->
                            <img class="absolute inset-0 w-full h-full object-cover" alt="A Pokémon" src={editedImage} />
                            <div class="absolute bottom-1 left-2 bg-white/30 backdrop-blur-md rounded-md px-1">
                                <span class="text-3xl">{editedName}</span>
                            </div>
                            <div class="absolute bottom-1 right-1">
                                <span class="badge preset-outlined-secondary-500 preset-tonal-secondary">
                                    #{Id}
                                </span>
                            </div>
                        </div>
                    </div>

                    <label class="label">
                        <span class="label-text">Name</span>
                        <input bind:value={editedName} class="input" type="text" placeholder="Pokemon Name" />
                    </label>
                    <FileUpload
                        allowDrop
                        name="example"
                        accept="image/*"
                        maxFiles={1}
                        label="Select or drag an image here of your Pokémon!"
                        maxFileSize={1024 * 1024 * 10}
                        onFileChange={generatePreview}
                        onFileReject={console.error}
                        classes="w-full">
                        {#snippet iconInterface()}
                            <Icon icon="mdi:image-add-outline" class="size-12" />
                        {/snippet}
                        {#snippet iconFile()}
                            <Icon icon="mdi:image-outline" class="size-4" />
                        {/snippet}
                        {#snippet iconFileRemove()}
                            <Icon icon="mdi:close" class="size-4" />
                        {/snippet}
                    </FileUpload>
                </article>
                <footer class="flex justify-end gap-4">
                    <button type="button" class="btn preset-tonal" onclick={modalClose}>Cancel</button>
                    <button type="button" class="btn preset-filled" onclick={saveEdit}>Confirm</button>
                </footer>
            {/snippet}
        </Modal>
        <button class="btn preset-tonal" onclick={deletePokemon}>Delete</button>
    </div>
</div>
