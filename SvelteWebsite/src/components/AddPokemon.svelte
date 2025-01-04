<script>
    import { FileUpload } from "@skeletonlabs/skeleton-svelte";
    import { Modal } from "@skeletonlabs/skeleton-svelte";
    import Icon from "@iconify/svelte";
    import { getContext } from "svelte";

    let addModelOpen = $state(false);
    let pokemonName = $state("");
    let base64Image = $state(null);
    let previewImageSrc = $state();

    let { pokemonList = $bindable([]) }  = $props();

    export const toast = getContext("toast");

    async function generatePreview(event) {
        const reader = new FileReader();

        const files = event.acceptedFiles || [];

        if (files.length > 0) {
            const selectedFile = files[0];
            base64Image = await new Promise((resolve, reject) => {
                reader.onload = () => resolve(reader.result);
                reader.onerror = () => reject(new Error("Failed to read file"));
                reader.readAsDataURL(selectedFile);
            });
        } else {
            base64Image = null;
        }
    }

    function modalClose() {
        addModelOpen = false;
    }

    async function addPokemon() {
        if (!pokemonName || !base64Image) {
            toast.create({
                title: "Warning",
                description: "Please provide both a name and an image!",
                type: "warning",
            });
            return;
        }

        try {
            const pokemonData = {
                Id: 0,
                Name: pokemonName,
                Image: base64Image, // Full Base64 data URL
            };

            const response = await fetch("/api/pokemon", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(pokemonData),
            });

            if (!response.ok) {
                throw new Error(`Failed to add Pokémon: ${response.statusText}`);
            }

            let pokemon = await response.json();

            pokemonList.push(pokemon);

            toast.create({
                title: "Success",
                description: "Pokémon added successfully!",
                type: "success",
            });
            addModelOpen = false;
        } catch (error) {
            console.error(error);
            toast.create({
                title: "Error",
                description: "An error occurred while adding the Pokémon!",
                type: "error",
            });
        }
    }
</script>

<Modal bind:open={addModelOpen} contentBase="card bg-surface-100-900 p-6 space-y-4 shadow-xl max-w-screen-sm w-full" backdropClasses="backdrop-blur-sm">
    {#snippet trigger()}
        <div class="card preset-filled-tertiary-50-950 size-72 p-4 relative overflow-hidden group">
            <!--Image-->
            <div class="absolute inset-0 transition-all duration-500 cursor-pointer filter grayscale-0 group-hover:grayscale">
                <img class="size-full object-cover" alt="A Pokemon" src="pikachu_question.jpg" />
            </div>
            <!--Add button-->
            <Icon
                icon="material-symbols:add-circle-rounded"
                class="size-24 text-tertiary-500 absolute inset-1/2 -translate-x-1/2 -translate-y-1/2 opacity-0 transition-opacity duration-500 group-hover:opacity-100 pointer-events-none" />
        </div>
    {/snippet}
    {#snippet content()}
        <header class="flex justify-between">
            <h2 class="h2">Add Pokemon</h2>
        </header>
        <article class="flex flex-col gap-4">
            <div class="mx-auto">
                <div class="card preset-filled-tertiary-50-950 size-72 p-4 relative overflow-hidden group">
                    <!-- Normal Display -->
                    <img class="absolute inset-0 w-full h-full object-cover" alt="A Pokémon" src={base64Image ? base64Image : "pokemon_question.png"} />
                    <div class="absolute bottom-1 left-2 bg-white/30 backdrop-blur-md rounded-md px-1">
                        <span class="text-3xl">{pokemonName}</span>
                    </div>
                </div>
            </div>

            <label class="label">
                <span class="label-text">Name</span>
                <input bind:value={pokemonName} class="input" type="text" placeholder="Pokemon Name" />
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
            <button type="button" class="btn preset-filled" onclick={addPokemon}>Confirm</button>
        </footer>
    {/snippet}
</Modal>
