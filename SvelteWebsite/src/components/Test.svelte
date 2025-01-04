<script>
    import { Tabs } from "@skeletonlabs/skeleton-svelte";

    let group = $state("plane");
    const lorem =
        "Lorem, ipsum dolor sit amet consectetur adipisicing elit. Nostrum veniam reprehenderit eum, reiciendis obcaecati, excepturi nemo ipsa fugit suscipit autem vitae numquam et cumque praesentium vero eos minus itaque. Lorem, ipsum dolor sit amet consectetur adipisicing elit. Nostrum veniam reprehenderit eum, reiciendis obcaecati, excepturi nemo.";

    function resolveAfter2Seconds() {
        return new Promise((resolve) => {
            setTimeout(() => {
                resolve("resolved");
            }, 2000);
        });
    }

    async function changeTab() {
        await resolveAfter2Seconds();
        group = group == "plane" ? "boat" : group == "boat" ? "car" : (group = "car" ? "plane" : "plane");
    }

    $inspect(group);
</script>

<button class="btn" onclick={changeTab}>Change Tab</button>

<Tabs bind:value={group}>
    {#snippet list()}
        <Tabs.Control value="plane">Plane</Tabs.Control>
        <Tabs.Control value="boat">Boat</Tabs.Control>
        <Tabs.Control value="car">Car</Tabs.Control>
    {/snippet}
    {#snippet content()}
        <Tabs.Panel value="plane">Plane Panel - {lorem}</Tabs.Panel>
        <Tabs.Panel value="boat">Boat Panel - {lorem}</Tabs.Panel>
        <Tabs.Panel value="car">Car Panel - {lorem}</Tabs.Panel>
    {/snippet}
</Tabs>
