<script>
    // Define the data prop
  /** @type {{data?: any}} */
  let { data = [] } = $props();
  </script>
  
  <div class="table-container overflow-x-auto">
    <table class="table table-hover table-compact w-full bg-white border rounded-lg shadow-md">
      <thead class="bg-gray-100">
        <tr class="text-left">
          <th class="p-3 text-gray-700 font-semibold">Key</th>
          <th class="p-3 text-gray-700 font-semibold">Value</th>
        </tr>
      </thead>
      <tbody>
        {#each data as row}
          <tr class="hover:bg-gray-50">
            <td class="p-3 border-b">{row.key}</td>
            <td class="p-3 border-b">{row.value}</td>
          </tr>
        {/each}
      </tbody>
    </table>
  </div>
  