<script>
    import AddPokemon from "../../components/AddPokemon.svelte";
    import Pokemon from "../../components/Pokemon.svelte";
    import { onMount } from "svelte";
    import { getContext } from "svelte";
    export const toast = getContext("toast");

    let pokemonList = $state([]);

    async function fetchPokemon()
    {
        try {
            const response = await fetch("/api/pokemon");
            if (!response.ok) {
                throw new Error(`Error fetching Pokémon: ${response.statusText}`);
            }
            const data = await response.json();

            // Validate and map the data to the expected format if needed
            pokemonList = data.map(({ Id, Name, Image }) => ({
                Id,
                Name,
                Image,
            }));
        } catch (err) {
            error = err.message;
            console.error(err);
            toast.create({
                title: "Error",
                description: "An error occurred while fetching Pokémons!",
                type: "error",
            });
        }
    }

    onMount(fetchPokemon);
</script>

<svelte:head>
    <title>Muppet Express - Pokémon</title>
</svelte:head>

<div class="py-4 flex flex-wrap gap-4">
    {#each pokemonList as pokemon}
    <Pokemon {...pokemon} bind:pokemonList={pokemonList}/>
    {/each}
    <AddPokemon bind:pokemonList={pokemonList} />
</div>
