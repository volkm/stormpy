{% if objname in template_functions(module) %}
{% include 'autosummary/templatefunction.rst' %}
{% else %}
{% include 'autosummary/function.rst' %}
{% endif %}
